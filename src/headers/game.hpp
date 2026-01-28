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
