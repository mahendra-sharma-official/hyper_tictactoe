#pragma once
#include "tictactoe.hpp"

class BigBoard
{
public:
    vector<int> winners;
    vector<TicTacToe> miniBoards;

    BigBoard();
    void CheckBoardWinners();
};

BigBoard::BigBoard() : winners(vector<int>(9, 0))
{

    miniBoards.reserve(9);
    for (int i = 0; i < 9; i++)
        miniBoards.push_back(TicTacToe());
}

void BigBoard::CheckBoardWinners()
{
    for (int i = 0; i < 9; i++)
        if (winners[i] == 0)
            winners[i] = miniBoards[i].CheckWinner();
}
