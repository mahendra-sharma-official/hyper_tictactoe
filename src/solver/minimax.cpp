#include "solver/minimax.hpp"

#include <iostream>

Minimax_Solver::Minimax_Solver() : maxDepth(1), playedTotalTurns(0) {}
static float WIN_SCORE = 100000.f;
static float LOSS_SCORE = -100000.f;
static float DRAW_SCORE = 0.f;

float EvaluateLine(int a, int b, int c, int rootPlayer) {
  int sum = a + b + c;
  int empty = (a == 0) + (b == 0) + (c == 0);

  if (empty != 1) return 0.f;

  if (sum == 2 * rootPlayer) return +10.f;
  if (sum == -2 * rootPlayer) return -12.f;

  return 0.f;
}

float EvaluateSmallBoard(const vector<int> &board, int rootPlayer) {
  float score = 0.f;

  for (int i = 0; i < 3; i++) {
    score += EvaluateLine(board[i * 3], board[i * 3 + 1], board[i * 3 + 2],
                          rootPlayer);
    score += EvaluateLine(board[i], board[i + 3], board[i + 6], rootPlayer);
  }

  score += EvaluateLine(board[0], board[4], board[8], rootPlayer);
  score += EvaluateLine(board[2], board[4], board[6], rootPlayer);

  return score;
}
float Minimax_Solver::EvaluateBigBoard(BigBoard &big, int rootPlayer) {
  float score = 0.f;

  for (int i = 0; i < 9; i++) {
    int w = big.winners[i];

    if (w == rootPlayer)
      score += 50.f;
    else if (w == -rootPlayer)
      score -= 50.f;
    else if (w == 0)
      score += EvaluateSmallBoard(big.miniBoards[i].board, rootPlayer);
    // draw boards (w == 2) contribute nothing
  }

  return score;
}

float TerminalScore(int winner, int rootPlayer, int depth) {
  if (winner == rootPlayer) return WIN_SCORE - depth;

  if (winner == -rootPlayer) return LOSS_SCORE + depth;

  return DRAW_SCORE;
}

vector<Index> Minimax_Solver::GetAvailableMoves(BigBoard &big,
                                                int playableIndex,
                                                vector<Index> &moves,
                                                int count_totalTurns) {
  // For only one miniboard (forced)
  if (playableIndex != -1 && big.miniBoards[playableIndex].winner == 0) {
    moves.reserve(9);
    for (int i = 0; i < 9; i++)
      if (big.miniBoards[playableIndex].board[i] == 0)
        moves.emplace_back(playableIndex, i);
  } else {
    moves.reserve(82 - count_totalTurns);
    // for any board (when index to play is already unplayable)
    for (int b = 0; b < 9; b++) {
      if (big.winners[b] != 0) continue;

      for (int i = 0; i < 9; i++)
        if (big.miniBoards[b].board[i] == 0) moves.emplace_back(b, i);
    }
  }

  return moves;
}
/*
float Minimax_Solver::Minimax(BigBoard &big_board, int depth, bool isMaximizing,
int playableIndex, int curr_player, int count_totalTurns)
{
    big_board.CheckBoardWinners();
    if (big_board.IsMoveLeft() == false || depth == 0)
        return EvaluateBigBoard(big_board, curr_player) * (1.f + depth * 2.f);

    float bestScore = isMaximizing ? -100000 : 100000;
    vector<Index> moves;
    GetAvailableMoves(big_board, playableIndex, moves, count_totalTurns +
(maxDepth - depth));

    for (auto &m : moves)
    {
        // Make move
        big_board.miniBoards[m.i].board[m.j] = isMaximizing ? curr_player :
-curr_player; big_board.miniBoards[m.i].CheckWinner();

        int nextPlayableIndex = m.j; // next board index is where current player
played

        // Determine next playable board in case of nonplayable board
        if (big_board.miniBoards[nextPlayableIndex].winner != 0)
            nextPlayableIndex = -1;

        float score = Minimax(big_board, depth - 1, !isMaximizing,
nextPlayableIndex, curr_player, count_totalTurns + 1);

        // Undo move
        big_board.miniBoards[m.i].board[m.j] = 0;

        // reseting the winner in case it had won (otherwise checkwinner will
not update properly) big_board.winners[m.i] = 0;
        big_board.miniBoards[m.i].winner = 0;
        big_board.miniBoards[m.i].CheckWinner();

        big_board.CheckBoardWinners(); // REQUIRED

        // big_board.miniBoards[m.i].CheckWinner();

        if (isMaximizing)
            bestScore = max(bestScore, score);
        else
            bestScore = min(bestScore, score);
    }
    return bestScore;
}

*/

float Minimax_Solver::Minimax(BigBoard &big, int depth, bool isMaximizing,
                              int playableIndex, int rootPlayer, int turnPlayer,
                              int count_totalTurns) {
  big.CheckBoardWinners();
  int winner = big.GetWinner();

  if (winner != 0) return TerminalScore(winner, rootPlayer, depth);

  if (depth == 0) return EvaluateBigBoard(big, rootPlayer);

  float best = isMaximizing ? -1e9f : 1e9f;

  vector<Index> moves;
  GetAvailableMoves(big, playableIndex, moves,
                    count_totalTurns + (maxDepth - depth));

  for (auto &m : moves) {
    // make move
    big.miniBoards[m.i].board[m.j] = turnPlayer;
    big.miniBoards[m.i].CheckWinner();
    big.CheckBoardWinners();

    int nextPlayable = m.j;
    if (big.miniBoards[nextPlayable].winner != 0) nextPlayable = -1;

    float score = Minimax(big, depth - 1, !isMaximizing, nextPlayable,
                          rootPlayer, -turnPlayer, count_totalTurns + 1);

    // undo move
    big.miniBoards[m.i].board[m.j] = 0;
    big.miniBoards[m.i].winner = 0;
    big.winners[m.i] = 0;
    big.miniBoards[m.i].CheckWinner();
    big.CheckBoardWinners();

    if (isMaximizing)
      best = max(best, score);
    else
      best = min(best, score);
  }

  return best;
}

Index Minimax_Solver::FindBestMove(BigBoard &big, int playableIndex,
                                   int currPlayer, int depth,
                                   int count_totalTurns) {
  float bestScore = -1e9f;
  Index bestMove(-1, -1);

  vector<Index> moves;
  GetAvailableMoves(big, playableIndex, moves, count_totalTurns);

  for (auto &mv : moves) {
    big.miniBoards[mv.i].board[mv.j] = currPlayer;
    big.miniBoards[mv.i].CheckWinner();
    big.CheckBoardWinners();

    float score = Minimax(big, depth - 1, false, mv.j,
                          currPlayer,   // root
                          -currPlayer,  // opponent
                          count_totalTurns + 1);

    // undo
    big.miniBoards[mv.i].board[mv.j] = 0;
    big.miniBoards[mv.i].winner = 0;
    big.winners[mv.i] = 0;
    big.CheckBoardWinners();

    if (score > bestScore) {
      bestScore = score;
      bestMove = mv;
    }
  }

  return bestMove;
}

/*
Index Minimax_Solver::FindBestMove(BigBoard &big_board, int playableIndex, int
curr_player, int depth, bool to_max, int count_totalTurns)
{
    float bestScore = -100000.f;
    Index bestMove(-1, -1);

    vector<Index> moves;
    GetAvailableMoves(big_board, playableIndex, moves, count_totalTurns);
    for (auto &mv : moves)
    {
        big_board.miniBoards[mv.i].board[mv.j] = curr_player;
        // int prev_winner = big_board.miniBoards[mv.i].winner; // previous
winner to revert the change big_board.miniBoards[mv.i].CheckWinner();

        float score = Minimax(big_board, depth - 1, to_max, mv.j, curr_player,
count_totalTurns + 1);

        big_board.miniBoards[mv.i].board[mv.j] = 0;
        big_board.winners[mv.i] = 0;
        big_board.miniBoards[mv.i].winner = 0;

        if (score > bestScore)
        {
            bestScore = score;
            bestMove = {mv.i, mv.j};
        }
    }

    return bestMove;
}
*/