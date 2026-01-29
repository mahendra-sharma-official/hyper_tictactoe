#include "tictactoe.hpp"

TicTacToe::TicTacToe() : winner(0), board(vector<int>(9, 0))
{
}

void TicTacToe::CheckWinner()
{
    // no need to check again if winner is already decided
    if (winner != 0)
        return;

    // rows and cols
    for (int i = 0; i < 3; i++)
    {
        // rows
        if ((board[i * 3] == board[i * 3 + 1]) && (board[i * 3 + 1] == board[i * 3 + 2]) && board[i * 3] != 0)
        {
            winner = board[i * 3];
            return;
        }
        // cols
        if ((board[i] == board[3 + i]) && (board[3 + i] == board[6 + i]) && board[i] != 0)
        {
            winner = board[i];
            return;
        }
    }

    // diagonals
    if ((board[0] == board[4]) && (board[4] == board[8]) && board[4] != 0)
    {
        winner = board[0];
        return;
    }
    else if ((board[2] == board[4]) && (board[4] == board[6]) && board[4] != 0)
    {
        winner = board[2];
        return;
    }

    if (!IsMoveLeft())
        winner = 2; // draw
}

bool TicTacToe::IsMoveLeft()
{
    for (int &v : board)
        if (v == 0)
            return true;
    return false;
}
