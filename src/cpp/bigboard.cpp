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
        {
            miniBoards[i].CheckWinner();
            winners[i] = miniBoards[i].winner;
        }
}

int BigBoard::GetWinner()
{
    vector<int> &board = winners;
    // rows and cols
    for (int i = 0; i < 3; i++)
    {
        // rows
        if ((board[i * 3] == board[i * 3 + 1]) && (board[i * 3 + 1] == board[i * 3 + 2]) && board[i * 3] != 0 && board[i * 3] != 2)
        {
            return board[i * 3];
        }
        // cols
        if ((board[i] == board[3 + i]) && (board[3 + i] == board[6 + i]) && board[i] != 0 && board[i] != 2)
        {
            return board[i];
        }
    }

    // diagonals
    if ((board[0] == board[4]) && (board[4] == board[8]) && board[4] != 0 && board[0] != 2)
    {
        return board[0];
    }
    else if ((board[2] == board[4]) && (board[4] == board[6]) && board[4] != 0 && board[0] != 2)
    {
        return board[2];
    }

    if (!IsMoveLeft())
        return 2; // draw
    return 0;
}
