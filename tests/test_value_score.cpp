#include "player/my_observer.hpp"
#include "player/my_player.hpp"
#include <cassert>
#include <iostream>

int test_value_score_main();

int main(int argc, char *argv[]) 
{
    return test_value_score_main();
}

int test_value_score_main()
{
    ttt::my_player::MyPlayer::initTables();
    auto board = ttt::my_player::MyPlayer::FastBoard();
    board.rows = 20;
    board.cols = 20;
    ttt::my_player::MyPlayer player("BlaBla");

    for (int y = 0; y < 20; ++y)
    {
        for (int x = 0; x < 20; ++x)
            board.set(x, y, ttt::game::Sign::NONE);
    }
    std::cout << "Testing my Value Score function\n";

    long long score = player.valueScore(board, ttt::game::Sign::X, 10, 10);
    assert(score < 1000);

    board.set(2, 5, ttt::game::Sign::X);
    board.set(3, 5, ttt::game::Sign::X);
    board.set(4, 5, ttt::game::Sign::X);
    
    score = player.valueScore(board, ttt::game::Sign::X, 5, 5);
    assert(score >= 20000);

    
    board.set(1, 5, ttt::game::Sign::X);
    score = player.valueScore(board, ttt::game::Sign::X, 5, 5);
    assert(score >= 1000000);

    board.set(1, 5, ttt::game::Sign::NONE);
    board.set(2, 5, ttt::game::Sign::NONE);

    long long score1 = player.valueScore(board, ttt::game::Sign::X, 5, 5);

    board.set(5, 3, ttt::game::Sign::X);
    board.set(5, 4, ttt::game::Sign::X);
    score = player.valueScore(board, ttt::game::Sign::X, 5, 5);
    assert(score >= score1 * 5);

    std::cout << "All is correct\n";
    return 0;
}