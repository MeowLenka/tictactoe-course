#include "my_player.hpp"
#include <cstdlib>

namespace ttt::my_player
{

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
