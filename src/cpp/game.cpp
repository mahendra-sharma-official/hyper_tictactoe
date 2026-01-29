#include "game.hpp"

Game::Game() : winner(0), currentTurn(1), running(false), playableIndex(-1), gameBoard(BigBoard())
{
}

void Game::Init()
{
    running = true;
    winner = 0;             // No winner
    currentTurn = 1;        // X plays first
    playableIndex = -1;     // -1 means no restriction on where you can play
    gameBoard = BigBoard(); // Reset the board
}

void Game::Stop()
{
    running = false;
}

void Game::Restart()
{
    Init();
}

void Game::CheckWinner()
{

    // no need to check again if winner is already decided
    if (winner != 0)
        return;

    // rows and cols
    for (int i = 0; i < 3; i++)
    {
        // rows
        if ((gameBoard.winners[i * 3] == gameBoard.winners[i * 3 + 1]) && (gameBoard.winners[i * 3 + 1] == gameBoard.winners[i * 3 + 2]) && gameBoard.winners[i * 3] != 0)
        {
            winner = gameBoard.winners[i * 3];
            return;
        }

        // cols
        if ((gameBoard.winners[i] == gameBoard.winners[3 + i]) && (gameBoard.winners[3 + i] == gameBoard.winners[6 + i]) && gameBoard.winners[i] != 0)
        {
            winner = gameBoard.winners[i];
            return;
        }
    }

    // diagonals
    if ((gameBoard.winners[0] == gameBoard.winners[4]) && (gameBoard.winners[4] == gameBoard.winners[8]) && gameBoard.winners[0] != 0)
    {
        winner = gameBoard.winners[4];
        return;
    }
    else if ((gameBoard.winners[2] == gameBoard.winners[4]) && (gameBoard.winners[4] == gameBoard.winners[6]) && gameBoard.winners[2] != 0)
    {
        winner = gameBoard.winners[4];
        return;
    }

    if (!gameBoard.IsMoveLeft()) // draw
        winner = 2;
}

bool Game::CanPlayOn(int i, int j)
{
    if (gameBoard.winners[i] != 0 && playableIndex == i)
    {
        playableIndex = -1;
        return false;
    }

    if (playableIndex == -1 && gameBoard.winners[i] != 0)
    {
        return false;
    }

    if ((playableIndex == -1 && gameBoard.winners[i] == 0) || (playableIndex == i & gameBoard.winners[i] == 0))
        return true;

    return false;
}

void Game::UpdatesOnClick(int i, int j)
{
    gameBoard.miniBoards[i].board[j] = currentTurn;

    // Checking full board's individual tictactoe winners
    gameBoard.CheckBoardWinners();
    // Checking the game's winner
    CheckWinner();
    currentTurn *= -1;
    if (gameBoard.winners[j] == 0)
        playableIndex = j;
    else
    {
        playableIndex = -1;
    }

    if (winner != 0)
    {
        Stop();
    }
}
