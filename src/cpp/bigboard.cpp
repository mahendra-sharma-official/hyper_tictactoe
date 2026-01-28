#include "bigboard.hpp"

BigBoard::BigBoard() : winners(vector<int>(9, 0))
{

    miniBoards.reserve(9);
    for (int i = 0; i < 9; i++)
        miniBoards.push_back(TicTacToe());
}

bool BigBoard::IsMoveLeft()
{
    for (int &v : winners)
        if (v == 0)
            return true;
    return false;
}

void BigBoard::CheckBoardWinners()
{
    for (int i = 0; i < 9; i++)
        if (winners[i] == 0)
            winners[i] = miniBoards[i].CheckWinner();
}