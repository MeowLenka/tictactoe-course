#include "player/my_observer.hpp"
#include "player/my_player.hpp"
#include <cassert>
#include <iostream>

int main(int argc, char *argv[])
{
    MyPlayer::FastBoard board;
    board.rows = 20;
    board.cols = 20;
    for (int y = 0; y < 20; ++y)
    {
        for (int x = 0; x < 20; ++x)
            board.set(x, y, Sign::NONE);
    }
    std::cout << "Testing my Value Score function\n";

    long long score = valueScore(board, Sign::X, 10, 10);
    assert(score < 100);

    board.set(2, 5, Sign::X);
    board.set(3, 5, Sign::X);
    board.set(4, 5, Sign::X);
    
    score = valueScore(board, Sign::X, 5, 5);
    assert(score >= 500000)

    long long scoreO = valueScore(board, Sign::O, 5, 5);
    assert(scoreO < score);
    
    board.set(1, 5, Sign::X);
    score = valueScore(board, Sign::X, 5, 5);
    assert(score >= 1000000000)

    board.set(1, 5, Sign::NONE);
    board.set(2, 5, Sign::NONE);

    long long score1 = valueScore(board, Sign::X, 5, 5);

    board.set(5, 3, Sign::X);
    board.set(5, 4, Sign::X);
    score = valueScore(boardSingle, Sign::X, 5, 5);

    assert(score >= score1 * 5);

    std::cout << "All is correct\n";
    return 0;
}