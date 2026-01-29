#pragma once
#include <vector>

using namespace std;

class TicTacToe
{
public:
    int winner;        // 0 -> not decided, 2 -> draw, 1-> X, -1 -> O
    vector<int> board; // 0 -> no one played, 1 -> X, -1 -> O

    TicTacToe();
    void CheckWinner();
    bool IsMoveLeft();
};
