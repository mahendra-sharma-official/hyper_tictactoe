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
    void Restart();
    void CheckWinner();
    bool CanPlayOn(int i, int j);
    void UpdatesOnClick(int i, int j);
};
