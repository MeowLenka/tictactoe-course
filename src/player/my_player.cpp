#include "my_player.hpp"
#include <cstdlib>

namespace ttt::my_player
{

  std::array<long long, 243> MyPlayer::s_patternScore;
  bool MyPlayer::s_tablesInitialized = false;

  static const long long WIN_SCORE = 1000000000LL;
  static const int ATTACK_COEFF = 4;                 // коэффициент для своих value
  static const int DEFENSE_COEFF = 2;                // коэффициент для чужих value
  static const double POSITION_DEFENSE_FACTOR = 0.8; // при оценке позиции

  void MyPlayer::set_sign(Sign sign) { m_sign = sign; }
  const char *MyPlayer::get_name() const { return m_name; }

  void MyPlayer::FastBoard::sync(const State &state)
  {
    rows = state.get_opts().rows;
    cols = state.get_opts().cols;
    for (int y = 0; y < rows; ++y)
    {
      for (int x = 0; x < cols; ++x)
      {
        grid[y][x] = state.get_value(x, y);
      }
    }
  }

  Sign MyPlayer::FastBoard::get(int x, int y) const
  {
    if (x < 0 || x >= cols || y < 0 || y >= rows)
      return Sign::WALL;
    return grid[y][x];
  }

  bool MyPlayer::FastBoard::isValid(int x, int y) const
  {
    return x >= 0 && x < cols && y >= 0 && y < rows;
  }

  void MyPlayer::FastBoard::set(int x, int y, Sign sign)
  {
    if (isValid(x, y))
      grid[y][x] = sign;
  }

  void MyPlayer::initTables()
  {
    if (s_tablesInitialized)
      return;

    for (int idx = 0; idx < 243; ++idx)
    {
      int temp = idx;
      std::array<int, 5> window;
      int ownCount = 0;
      int emptyCount = 0;
      int blockedCount = 0;

      for (int i = 0; i < 5; ++i)
      {
        window[i] = temp % 3;
        if (window[i] == 1)
          ownCount++; // наш символ
        else if (window[i] == 0)
          emptyCount++; // пусто
        else
          blockedCount++; // чужой символ или стена
        temp /= 3;
      }
      // если есть чужой или стена внутри окна -> оценка 0
      if (blockedCount > 0)
      {
        s_patternScore[idx] = 0;
        continue;
      }
      // дальше идет оценка шаблонов
      if (ownCount == 5)
        s_patternScore[idx] = WIN_SCORE;

      else if (ownCount == 4 && emptyCount == 1)
        s_patternScore[idx] = 500000LL; // открытая четверка

      else if (ownCount == 4)
        s_patternScore[idx] = 50000LL; // закрытая четверка

      else if (ownCount == 3 && emptyCount == 2)
      {
        if (window[0] == 0 && window[4] == 0)
          s_patternScore[idx] = 20000LL; // открытая по краям тройка
        else
          s_patternScore[idx] = 5000LL; // полуоткрытая тройка
      }

      else if (ownCount == 2 && emptyCount == 3)
      {
        if (window[0] == 0 && window[4] == 0)
          s_patternScore[idx] = 500LL; // открытая двойка
        else
          s_patternScore[idx] = 100LL; // полуоткрытая двойка
      }

      else if (ownCount == 1 && emptyCount == 4)
        s_patternScore[idx] = 30LL;

      else
        s_patternScore[idx] = 0;
    }

    s_tablesInitialized = true;
  }

  int MyPlayer::windowToIndex(const std::array<int, 5> &window)
  {
    int idx = 0;
    int power = 1;
    for (int i = 0; i < 5; ++i)
    {
      idx += window[i] * power;
      power *= 3;
    }
    return idx;
  }

  void MyPlayer::buildLine(const FastBoard &board, Sign player, int x, int y,
                           int dx, int dy, std::array<int, 9> &line) const
  {
    for (int k = -4; k <= 4; ++k)
    {
      int idx = k + 4;
      if (k == 0)
      {
        line[idx] = 1;
        continue;
      }

      int nx = x + k * dx;
      int ny = y + k * dy;
      Sign val = board.get(nx, ny);

      if (val == player)
        line[idx] = 1; // свой символ
      else if (val == Sign::NONE)
        line[idx] = 0; // пусто
      else
        line[idx] = 2; // чужой или стена
    }
  }

  long long MyPlayer::scoreLine(const std::array<int, 9> &line) const
  {
    long long score = 0;
    // окна по 5 клеток
    for (int i = 0; i < 5; ++i)
    {
      std::array<int, 5> window;
      for (int j = 0; j < 5; ++j)
        window[j] = line[i + j];

      int idx = windowToIndex(window);
      score += s_patternScore[idx];
    }
    return score;
  }

  long long MyPlayer::valueScore(const FastBoard &board, Sign player, int x, int y) const
  {
    if (board.get(x, y) != Sign::NONE)
      return 0;

    const int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    long long totalScore = 0;
    int valueCount = 0;

    for (const auto &dir : directions)
    {
      std::array<int, 9> line;
      buildLine(board, player, x, y, dir[0], dir[1], line);
      long long segmentScore = scoreLine(line);

      if (segmentScore >= 5000) // открытая четвёрка или тройка
      {
        valueCount++;
      }
      totalScore += segmentScore;
    }
    if (valueCount >= 2)
    {
      totalScore *= 10;
    }
    return totalScore;
  }

  bool MyPlayer::isPromising(const FastBoard &board, int x, int y) const
  {
    for (int dy = -2; dy <= 2; ++dy)
    {
      for (int dx = -2; dx <= 2; ++dx)
      {
        if (dx == 0 && dy == 0)
          continue;

        Sign val = board.get(x + dx, y + dy);
        if (val == Sign::X || val == Sign::O)
          return true;
      }
    }
    return false;
  }

  int MyPlayer::centerBonus(int x, int y, int moveNumber) const
  {
    if (moveNumber >= 4)
      return 0;

    int centerX = 10;
    int centerY = 10;
    int distance = std::abs(x - centerX) + std::abs(y - centerY);
    int bonus = 4 - distance;
    return bonus > 0 ? bonus : 0;
  }

  int MyPlayer::obstaclePenalty(const FastBoard &board, int x, int y) const
  {
    int penalty = 0;
    for (int dy = -2; dy <= 2; ++dy)
    {
      for (int dx = -2; dx <= 2; ++dx)
      {
        if (dx == 0 && dy == 0)
          continue;
        int nx = x + dx;
        int ny = y + dy;
        Sign val = board.get(nx, ny);

        if (val == Sign::WALL)
        {
          int distance = std::abs(dx) + std::abs(dy);
          penalty += (4 - distance) * 5;
        }
      }
    }
    return penalty;
  }

  long long MyPlayer::evaluateCell(const FastBoard &board, int x, int y,
                                   const ClusterInfo &cluster, int moveNumber) const
  {
    if (board.get(x, y) != Sign::NONE)
      return -1e18;

    Sign opponent = (m_sign == Sign::X) ? Sign::O : Sign::X;

    long long myValue = valueScore(board, m_sign, x, y);
    long long oppValue = valueScore(board, opponent, x, y);

    // оценка
    long long score = ATTACK_COEFF * myValue + DEFENSE_COEFF * oppValue;

    score += centerBonus(x, y, moveNumber);
    score -= obstaclePenalty(board, x, y);

    // + за близость к центру кластера
    if (cluster.valid)
    {
      int distToCluster = std::abs(x - cluster.center_x) + std::abs(y - cluster.center_y);
      if (distToCluster <= 3)
        score += 100 * (4 - distToCluster);
    }
    return score;
  }

  long long MyPlayer::evaluatePosition(const FastBoard &board, Sign current) const
  {
    long long myScore = 0;
    long long oppScore = 0;
    Sign opponent = (current == Sign::X) ? Sign::O : Sign::X;

    for (int y = 0; y < board.rows; ++y)
    {
      for (int x = 0; x < board.cols; ++x)
      {
        if (board.get(x, y) == Sign::NONE && isPromising(board, x, y))
        {
          myScore += valueScore(board, current, x, y);
          oppScore += valueScore(board, opponent, x, y);
        }
      }
    }
    return myScore - static_cast<long long>(oppScore * POSITION_DEFENSE_FACTOR);
  }

   bool MyPlayer::hasLineAfterMove(const FastBoard &board, int x, int y, Sign player) const
  {
    FastBoard copy = board;
    copy.set(x, y, player);

    const int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    for (const auto &dir : directions)
    {
      int count = 1;
      for (int i = 1; i <= 4; ++i)
      {
        Sign val = copy.get(x + i * dir[0], y + i * dir[1]);
        if (val == player)
          count++;
        else
          break;
      }
      for (int i = 1; i <= 4; ++i)
      {
        Sign val = copy.get(x - i * dir[0], y - i * dir[1]);
        if (val == player)
          count++;
        else
          break;
      }

      if (count >= WIN_LENGTH)
        return true;
    }
    return false;
  }

  bool MyPlayer::isRealXWin(const FastBoard &board, int x, int y) const 
  {
    if (!hasLineAfterMove(board, x, y, Sign::X))
      return false;

    FastBoard afterX = board;
    afterX.set(x, y, Sign::X);

    // если поле заполнено, O не может ответить
    int freeCount = 0;
    for (int i = 0; i < afterX.rows; ++i)
    {
      for (int j = 0; j < afterX.cols; ++j)
      {
        if (afterX.get(j, i) == Sign::NONE)
          freeCount++;
      }
    }
    if (freeCount == 0)
      return true;

    // может ли O ответить победой
    for (int oy = 0; oy < afterX.rows; ++oy)
    {
      for (int ox = 0; ox < afterX.cols; ++ox)
      {
        if (afterX.get(ox, oy) == Sign::NONE)
        {
          if (hasLineAfterMove(afterX, ox, oy, Sign::O))
            return false;
        }
      }
    }
    return true;
  }

   bool MyPlayer::isXDraw(const FastBoard &board, int x, int y) const
  {
    if (!hasLineAfterMove(board, x, y, Sign::X))
      return false;

    FastBoard afterX = board;
    afterX.set(x, y, Sign::X);

    for (int oy = 0; oy < afterX.rows; ++oy)
    {
      for (int ox = 0; ox < afterX.cols; ++ox)
      {
        if (afterX.get(ox, oy) == Sign::NONE)
        {
          if (hasLineAfterMove(afterX, ox, oy, Sign::O))
            return true;
        }
      }
    }
    return false;
  }

  Point MyPlayer::chooseFirstMove(const FastBoard &board, const ClusterInfo &cluster) const
  {
    Point best = {0, 0};
    long long bestScore = -1e18;

    for (int y = 0; y < board.rows; ++y)
    {
      for (int x = 0; x < board.cols; ++x)
      {
        if (board.get(x, y) != Sign::NONE)
          continue;

        long long score = valueScore(board, m_sign, x, y);
        score -= obstaclePenalty(board, x, y);

        if (cluster.valid)
        {
          int dist = std::abs(x - cluster.center_x) + std::abs(y - cluster.center_y);
          score -= dist * 10;
        }

        if (score > bestScore)
        {
          bestScore = score;
          best = {x, y};
        }
      }
    }
    return best;
  }

  Point MyPlayer::make_move(const State &state)
  {
    Point result;
    for (int n_attempt = 0; n_attempt < 50; ++n_attempt)
    {
      result.x = std::rand() % state.get_opts().cols;
      result.y = std::rand() % state.get_opts().rows;
      if (state.get_value(result.x, result.y) != Sign::NONE)
      {
        --n_attempt;
        continue;
      }
      bool has_neighbors = false;
      for (int dx = -1; dx <= 1; ++dx)
      {
        for (int dy = -1; dy <= 1; ++dy)
        {
          if (dx == 0 && dy == 0)
            continue;
          const Sign val = state.get_value(result.x + dx, result.y + dy);
          if (val == Sign::X || val == Sign::O)
          {
            has_neighbors = true;
            break;
          }
        }
        if (has_neighbors)
          break;
      }
      if (has_neighbors)
        break;
    }
    return result;
  }

}; // namespace ttt::my_player
