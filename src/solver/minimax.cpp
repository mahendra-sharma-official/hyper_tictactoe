#include "solver/minimax.hpp"

#include <iostream>

Minimax_Solver::Minimax_Solver() : maxDepth(1), playedTotalTurns(0) {}
static float WIN_SCORE = 100000.f;
static float LOSS_SCORE = -100000.f;
static float DRAW_SCORE = -5.f;

static float EvaluateLine(int a, int b, int c, int rootPlayer) {
  int sum = a + b + c;
  int empty = (a == 0) + (b == 0) + (c == 0);

  if (empty == 2) return +5.f;

  if (sum == 2 * rootPlayer) return +15.f;
  if (sum == -2 * rootPlayer) return -18.f;

  return 0.f;
}

static float EvaluateSmallBoard(const vector<int> &board, int rootPlayer) {
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

  // Try to win in winnable order in big board pattern as well
  // more valuable than winning in small board
  score += EvaluateSmallBoard(big.winners, rootPlayer) * 3.f;

  for (int i = 0; i < 9; i++) {
    int w = big.winners[i];

    if (w == rootPlayer)
      score += 100.f;
    else if (w == -rootPlayer)
      score -= 120.f;
    else if (w == 0)
      score += EvaluateSmallBoard(big.miniBoards[i].board, rootPlayer) * 1.f;
    else if (w == 2)
      score += -25.f;
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

float Minimax_Solver::Minimax(BigBoard &big, int depth, bool isMaximizing,
                              int playableIndex, int rootPlayer, int turnPlayer,
                              float alpha, float beta, int count_totalTurns) {
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

    int nextPlayable = m.j;
    if (big.miniBoards[nextPlayable].winner != 0) nextPlayable = -1;

    float score =
        Minimax(big, depth - 1, !isMaximizing, nextPlayable, rootPlayer,
                -turnPlayer, alpha, beta, count_totalTurns + 1);

    // undo move
    big.miniBoards[m.i].board[m.j] = 0;
    big.miniBoards[m.i].winner = 0;
    big.winners[m.i] = 0;

    if (isMaximizing) {
      best = max(best, score);
      alpha = max(alpha, best);
      if (alpha <= beta) break;
    } else {
      best = min(best, score);
      beta = min(beta, best);
      if (beta <= alpha) break;
    }
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
                          -1e9f, 1e9f, count_totalTurns + 1);

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