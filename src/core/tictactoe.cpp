#include "core/tictactoe.hpp"

TicTacToe::TicTacToe() : winner(0), board(vector<int>(9, 0)) {}

void TicTacToe::CheckWinner() {
  // no need to check again if winner is already decided
  if (winner != 0) return;

  // rows and cols
  for (int i = 0; i < 3; i++) {
    // rows
    if (AreElementsAtIndicesEqual(i * 3, i * 3 + 1, i * 3 + 2) &&
        board[i * 3] != 0) {
      winner = board[i * 3];
      return;
    }
    // cols
    if (AreElementsAtIndicesEqual(i, i + 3, i + 6) && board[i] != 0) {
      winner = board[i];
      return;
    }
  }

  // diagonals
  if (AreElementsAtIndicesEqual(0, 4, 8) && board[4] != 0) {
    winner = board[4];
    return;
  } else if (AreElementsAtIndicesEqual(2, 4, 6) && board[4] != 0) {
    winner = board[4];
    return;
  }

  if (!IsMoveLeft()) winner = 2;  // draw
}

bool TicTacToe::IsMoveLeft() {
  for (int &v : board)
    if (v == 0) return true;
  return false;
}

// Checks if elements at the given indices are equal
bool TicTacToe::AreElementsAtIndicesEqual(int i, int j, int k) {
  if (board[i] == board[j] && board[j] == board[k]) return true;
  return false;
}