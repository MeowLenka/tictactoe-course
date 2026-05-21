#include "my_player.hpp"
#include <cstdlib>

namespace ttt::my_player
{
  static const long long WIN_SCORE = 1000000000LL;

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

  long long MyPlayer::scoreLineSegment(const std::array<int, 9> &line) const
  {
    long long score = 0;
    for (int i = 0; i < 5; ++i)
    {
      std::array<int, 5> window;
      for (int j = 0; j < 5; ++j)
      {
        window[j] = line[i + j];
      }
      int idx = windowToIndex(window);
      score += s_patternScore[idx];
    }

    return score;
  }

}; // namespace ttt::my_player
