#pragma once
#include "core/tictactoe.hpp"

class BigBoard {
 public:
  vector<int> winners;
  vector<TicTacToe> miniBoards;

  BigBoard();
  bool IsMoveLeft();
  void CheckBoardWinners();
  int GetWinner();
};
