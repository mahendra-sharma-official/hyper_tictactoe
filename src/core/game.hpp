#pragma once
#include "core/bigboard.hpp"
#include "misc/index.hpp"
#include "solver/minimax.hpp"

class Game {
 public:
  int winner;
  int currentTurn;
  int playableIndex;
  int solverDepth;
  int playedTotalTurns;
  bool running;

  // Solver Related
  Minimax_Solver solver;
  bool toggleAutoplay;
  bool toggleSolveO;
  bool toggleSolveX;
  Index bestMoveX;
  Index bestMoveO;

  BigBoard gameBoard;

  Game();

  void Init();
  void Stop();
  void Restart();
  void CheckWinner();
  bool CanPlayOn(int i, int j);
  void UpdatesOnClick(int i, int j);
};
