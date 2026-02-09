#include "core/game.hpp"

Game::Game()
    : winner(0),
      currentTurn(1),
      running(false),
      playableIndex(-1),
      solverDepth(1),
      playedTotalTurns(0),
      gameBoard(BigBoard()) {}

void Game::Init() {
  running = true;
  winner = 0;          // No winner
  currentTurn = 1;     // X plays first
  playableIndex = -1;  // -1 means no restriction on where you can play
  playedTotalTurns = 0;
  gameBoard = BigBoard();  // Reset the board
}

void Game::Stop() { running = false; }

void Game::Restart() { Init(); }

void Game::CheckWinner() {
  // no need to check again if winner is already decided
  if (winner != 0) return;

  vector<int>& winners_ref = gameBoard.winners;
  // rows and cols
  for (int i = 0; i < 3; i++) {
    // rows
    if ((winners_ref[i * 3] == winners_ref[i * 3 + 1]) &&
        (winners_ref[i * 3 + 1] == winners_ref[i * 3 + 2]) &&
        (winners_ref[i * 3] != 0 && winners_ref[i * 3] != 2)) {
      winner = winners_ref[i * 3];
      return;
    }

    // cols
    if ((winners_ref[i] == winners_ref[3 + i]) &&
        (winners_ref[3 + i] == winners_ref[6 + i]) &&
        (winners_ref[i] != 0 && winners_ref[i] != 2)) {
      winner = winners_ref[i];
      return;
    }
  }

  // diagonals
  if ((winners_ref[0] == winners_ref[4]) &&
      (winners_ref[4] == winners_ref[8]) &&
      (winners_ref[4] != 0 && winners_ref[4] != 2)) {
    winner = winners_ref[4];
    return;
  } else if ((winners_ref[2] == winners_ref[4]) &&
             (winners_ref[4] == winners_ref[6]) &&
             (winners_ref[4] != 0 && winners_ref[4] != 2)) {
    winner = winners_ref[4];
    return;
  }

  if (!gameBoard.IsMoveLeft())  // draw
    winner = 2;
  return;
}

bool Game::CanPlayOn(int i, int j) {
  if (gameBoard.winners[i] != 0 && playableIndex == i) {
    playableIndex = -1;
    return false;
  }

  if (playableIndex == -1 && gameBoard.winners[i] != 0) {
    return false;
  }

  if ((playableIndex == -1 && gameBoard.winners[i] == 0) ||
      (playableIndex == i && gameBoard.winners[i] == 0))
    return true;

  return false;
}

void Game::UpdatesOnClick(int i, int j) {
  gameBoard.miniBoards[i].board[j] = currentTurn;
  playedTotalTurns++;

  // Checking full board's individual tictactoe winners
  gameBoard.CheckBoardWinners();
  // Checking the game's winner
  CheckWinner();
  currentTurn *= -1;
  if (gameBoard.winners[j] == 0)
    playableIndex = j;
  else {
    playableIndex = -1;
  }

  if (winner != 0) {
    Stop();
  }
}