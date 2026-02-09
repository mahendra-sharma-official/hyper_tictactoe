#pragma once
#include "core/bigboard.hpp"
#include "core/tictactoe.hpp"
#include "misc/index.hpp"

class Minimax_Solver {
 public:
  int maxDepth;
  int playedTotalTurns;
  Minimax_Solver();

  float EvaluateBigBoard(BigBoard &big_board, int curr_player);
  vector<Index> GetAvailableMoves(BigBoard &big_board, int playableIndex,
                                  vector<Index> &moves, int count_totalTurns);
  float Minimax(BigBoard &big, int depth, bool isMaximizing, int playableIndex,
                int rootPlayer, int turnPlayer, float alpha, float beta, int count_totalTurns);
  Index FindBestMove(BigBoard &big, int playableIndex, int currPlayer,
                     int depth, int count_totalTurns);
};