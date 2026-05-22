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
  // public:
    Sign m_sign = Sign::NONE;
    const char *m_name;

    static std::array<long long, 243> s_patternScore;
    static bool s_tablesInitialized;
    static void initTables();
    static int windowToIndex(const std::array<int, 5> &window);

    struct FastBoard
    {
      std::array<std::array<Sign, 20>, 20> grid;
      int rows, cols;

      void sync(const State &state);
      Sign get(int x, int y) const;
      void set(int x, int y, Sign sign);
      bool isValid(int x, int y) const;
    };

    struct ClusterInfo {
        bool valid = false;
        int center_x = 0;
        int center_y = 0;
        int size = 0;
    };
    
    long long scoreLine(const std::array<int, 9> &line) const;
    void buildLine(const FastBoard &board, Sign player, int x, int y, int dx, int dy, std::array<int, 9> &line) const;
    long long valueScore(const FastBoard &board, Sign player, int x, int y) const;

    bool isPromising(const FastBoard& board, int x, int y) const;
    int centerBonus(int x, int y, int moveNumber) const;
    int obstaclePenalty(const FastBoard& board, int x, int y) const;
    long long evaluateCell(const FastBoard& board, int x, int y, const ClusterInfo& cluster, int moveNumber) const;
    

  public:
    MyPlayer(const char *name) : m_sign(Sign::NONE), m_name(name) {}
    void set_sign(Sign sign) override;
    Point make_move(const State &game) override;
    const char *get_name() const override;
  };

}; // namespace ttt::my_player
