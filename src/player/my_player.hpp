#pragma once

#include "core/game.hpp"
#include <array>

namespace ttt::my_player
{
  using game::Event;
  using game::IPlayer;
  using game::Point;
  using game::Sign;
  using game::State;

  class MyPlayer : public IPlayer
  {
    Sign m_sign = Sign::NONE;
    const char *m_name;
  
    static std::array<long long, 243> s_patternScore;
    static bool s_tablesInitialized;
  
    struct FastBoard
    {
      std::array<std::array<Sign, 20>, 20> grid;
      int rows, cols;

      void sync(const State &state);
      Sign get(int x, int y) const;
      void set(int x, int y, Sign sign);
      bool isValid(int x, int y) const;
    };
  
  public:
    MyPlayer(const char *name) : m_sign(Sign::NONE), m_name(name) {}
    void set_sign(Sign sign) override;
    Point make_move(const State &game) override;
    const char *get_name() const override;
  };

}; // namespace ttt::my_player
