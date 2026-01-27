#pragma once
#include "bigboard.hpp"
#include <iostream>

class Game
{
public:
    int winner;
    int currentTurn;
    int playableIndex;
    bool running;

    BigBoard gameBoard;

    Game();

    void Init();
    void Stop();
    void Update();
    void Restart();
    int CheckWinner();
    bool CanPlayOn(int i, int j, int &prev);
    void UpdatesOnClick(int i, int j);
};

Game::Game() : winner(0), currentTurn(1), running(false), playableIndex(-1), gameBoard(BigBoard())
{
}

inline void Game::Init()
{
    running = true;
    winner = 0;             // Reset the winner
    currentTurn = 1;        // X plays first
    playableIndex = -1;     // -1 means no restriction on where you can play
    gameBoard = BigBoard(); // Reset the board
}

inline void Game::Stop()
{
    running = false;
}

inline void Game::Update()
{
}

inline void Game::Restart()
{
    Init();
}

int Game::CheckWinner()
{

    // no need to check again if winner is already decided
    if (winner != 0)
        return winner;

    // rows and cols
    for (int i = 0; i < 3; i++)
    {
        // rows
        if (gameBoard.winners[i * 3] == gameBoard.winners[i * 3 + 1] && gameBoard.winners[i * 3 + 1] == gameBoard.winners[i * 3 + 2])
            return gameBoard.winners[i * 3];
        // cols
        if (gameBoard.winners[i] == gameBoard.winners[3 + i] && gameBoard.winners[3 + i] == gameBoard.winners[6 + i])
            return gameBoard.winners[i];
    }

    // diagonals
    if (gameBoard.winners[0] == gameBoard.winners[4] && gameBoard.winners[4] == gameBoard.winners[8])
        return gameBoard.winners[0];
    else if (gameBoard.winners[2] == gameBoard.winners[4] && gameBoard.winners[4] == gameBoard.winners[6])
        return gameBoard.winners[4];

    return 0;
}

bool Game::CanPlayOn(int i, int j, int &prev)
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
    winner = CheckWinner();
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
