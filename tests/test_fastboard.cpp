#include "player/my_observer.hpp"
#include "player/my_player.hpp"
#include <cassert>
#include <iostream>

int test_fastboard_main();

int main(int argc, char *argv[])
{
    return test_fastboard_main();
}

int test_fastboard_main()
{
    ttt::game::State::Opts opts;
    opts.rows = opts.cols = 20;
    opts.win_len = 5;
    opts.max_moves = 0;

    auto random_fi = ttt::game::RandomObstaclesFI(0.75, 50, 1);
    auto state = ttt::game::State(opts, &random_fi);
    auto board = ttt::my_player::MyPlayer::FastBoard();
    board.sync(state);

    std::cout << "Testing my FastBoard\n";

    for (int y = 0; y < opts.rows; y++)
    {
        for (int x = 0; x < opts.cols; x++)
        {
            assert(state.get_value(x, y) == board.get(x, y));
        }
    }

    board.set(6, 7, ttt::game::Sign::X);
    assert(board.get(6, 7) == ttt::game::Sign::X);

    board.set(4, 2, ttt::game::Sign::O);
    assert(board.get(4, 2) == ttt::game::Sign::O);

    board.set(5, 2, ttt::game::Sign::NONE);
    assert(board.get(5, 2) == ttt::game::Sign::NONE);

    board.set(1, 3, ttt::game::Sign::WALL);
    assert(board.get(1, 3) == ttt::game::Sign::WALL);

    board.set(1, 4, ttt::game::Sign::X);
    assert(board.get(1, 4) == ttt::game::Sign::X);

    board.set(8, 8, ttt::game::Sign::O);
    assert(board.get(8, 8) == ttt::game::Sign::O);

    board.set(0, 0, ttt::game::Sign::NONE);
    assert(board.get(0, 0) == ttt::game::Sign::NONE);

    board.set(19, 19, ttt::game::Sign::WALL);
    assert(board.get(19, 19) == ttt::game::Sign::WALL);

    std::cout << "All is correct\n";
    return 0;
}