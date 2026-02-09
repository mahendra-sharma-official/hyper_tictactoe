#include "autoplay/autoplay.hpp"

void AutoPlay(Game &game, Game_Gui &gui, bool tglX, bool tglO) {
  game.toggleSolveX = tglX;
  game.toggleSolveO = tglO;

  BigBoard temp = game.gameBoard;
  int i = -1;
  int j = -1;
  // Run Solver
  if (game.toggleSolveX && game.currentTurn == 1) {
    game.bestMoveX =
        game.solver.FindBestMove(temp, game.playableIndex, game.currentTurn,
                                 game.solverDepth, game.playedTotalTurns);
    i = game.bestMoveX.i;
    j = game.bestMoveX.j;
  } else if (game.toggleSolveO && game.currentTurn == -1) {
    game.bestMoveO =
        game.solver.FindBestMove(temp, game.playableIndex, game.currentTurn,
                                 game.solverDepth, game.playedTotalTurns);

    i = game.bestMoveO.i;
    j = game.bestMoveO.j;
  }
  gui.HighlightBestMove();
  if (game.CanPlayOn(i, j)) {
    gui.miniBoardText[i][j].setString(game.currentTurn == 1 ? "X" : "O");
    game.UpdatesOnClick(i, j);  // Game's actual main logic starts here

    gui.UpdateGuiOnClick(
        i, j, gui.prevPlayableIndex);  // Updates gui that needs to be updated
                                       // after valid clicking
  }
}