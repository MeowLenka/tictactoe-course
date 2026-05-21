#include "player/my_observer.hpp"
#include "player/my_player.hpp"
#include <cassert>
#include <iostream>

int main(int argc, char *argv[])
{
    ttt::my_player::MyPlayer::initTables();
    ttt::my_player::MyPlayer::FastBoard board;
    ttt::my_player::MyPlayer player("BlaBla");
    board.rows = 20;
    board.cols = 20;
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

    long long scoreO = player.valueScore(board, ttt::game::Sign::O, 5, 5);
    assert(scoreO < score);
    
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