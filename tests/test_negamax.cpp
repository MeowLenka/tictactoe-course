#include "player/my_observer.hpp"
#include "player/my_player.hpp"
#include <cassert>
#include <iostream>

void clearBoard(ttt::my_player::MyPlayer::FastBoard &board)
{
    for (int y = 0; y < 20; ++y)
    {
        for (int x = 0; x < 20; ++x)
        {
            board.set(x, y, ttt::game::Sign::NONE);
        }
    }
}

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
    std::cout << "Testing my negamax\n";

    long long score = player.negamax(board, 0, -1e12, 1e12, ttt::game::Sign::X, -1, -1, 1);
    assert(score == 0);

    board.set(10, 10, ttt::game::Sign::X);
    score = player.negamax(board, 0, -1e12, 1e12, ttt::game::Sign::X, -1, -1, 1);
    assert(score > 0 && score < 20000);

    clearBoard(board);
    board.set(2, 5, ttt::game::Sign::X);
    board.set(3, 5, ttt::game::Sign::X);
    board.set(4, 5, ttt::game::Sign::X);
    score = player.negamax(board, 0, -1e12, 1e12, ttt::game::Sign::X, -1, -1, 1);
    assert(score > 500000);

    clearBoard(board);
    for (int i = 0; i < 4; ++i)
        board.set(i, 5, ttt::game::Sign::X); // 4 X подряд
    score = player.negamax(board, 0, -1e12, 1e12, ttt::game::Sign::X, -1, -1, 1);
    assert(score > 100000000);  

    clearBoard(board);
    for (int i = 0; i < 4; ++i)
        board.set(i, 5, ttt::game::Sign::O);
    score = player.negamax(board, 0, -1e12, 1e12, ttt::game::Sign::X, -1, -1, 1);
    assert(score < -100000000);


    clearBoard(board);
    board.set(0, 5, ttt::game::Sign::X);
    board.set(1, 5, ttt::game::Sign::X);
    board.set(3, 3, ttt::game::Sign::O);

    score = player.negamax(board, 0, -1e12, 1e12, ttt::game::Sign::X, -1, -1, 1);
    assert(score > 10000);

    clearBoard(board);
    int depth = player.getDynamicDepth(board, ttt::game::Sign::X);
    assert(depth == 3);

    // открытая тройка -> глубина 4
    board.set(2, 5, ttt::game::Sign::X);
    board.set(3, 5, ttt::game::Sign::X);
    board.set(4, 5, ttt::game::Sign::X);
    depth = player.getDynamicDepth(board, ttt::game::Sign::X);
    assert(depth == 4);

    // открытая четвёрка -> глубина 5
    clearBoard(board);
    for (int i = 0; i < 4; ++i)
    {
        board.set(i, 5, ttt::game::Sign::X);
    }
    depth = player.getDynamicDepth(board, ttt::game::Sign::X);
    assert(depth == 5);

    std::cout << "All is correct\n";
    return 0;
}