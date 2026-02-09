#include "gui/game_gui.hpp"

#include <iostream>

#include "gui/gui_helpers.hpp"

Game_Gui::Game_Gui(Game &g, RenderWindow &w)
    : game(g),
      window(w),
      txt_font("res/fonts/craftycandy.otf"),
      xo_font("res/fonts/craftycandy.otf"),
      playerTurnText(txt_font),
      restartText(txt_font),
      solveXText(txt_font),
      solveOText(txt_font),
      winnerText(txt_font) {
  prevPlayableIndex = -1;
  game.toggleSolveO = false;
  game.toggleSolveX = false;
  hud_width = 100;
  backgroundColor = Color::Black;
  miniBoardText.assign(9, vector<Text>(9, xo_font));
  bigBoardText.assign(9, xo_font);
}

void Game_Gui::Init(int mrgns, int thkns, int hudWdth) {
  // VALUES NEEDED TO INITIALIZE THE GAME BOARD
  mainContainerSize = window.getSize().y < window.getSize().x
                          ? window.getSize().y
                          : window.getSize().x;
  mainContainerSize -= (hudWdth * 2 + mrgns * 2);
  margin = mrgns;
  thickness = thkns;
  hud_width = hudWdth;
  game.toggleSolveO = false;
  game.toggleSolveX = false;

  // For Main board
  float l0 = mainContainerSize;
  float m0 = margin;
  float t0 = thickness;
  Vector2f off0{(window.getSize().x - l0) / 2.f,
                (window.getSize().y - l0) / 2.f};
  offset = off0;

  // For big boards
  float m1 = 3.5f * m0;
  float l1 = l0 / 3.f;
  float t1 = thickness;
  Vector2f off1 = off0;

  // For mini boards
  float m2 = 1.f;
  float l2 = (l1 - 2.f * m1 + 2 * m2) / 3.f;

  // Fonts and text
  int big_board_font_size = (1.10f * l1);
  int mini_board_font_size = (0.8f * l2);
  int hud_font_size = mini_board_font_size;

  // To center the big texts
  Vector2f txtOff_big{l1 / 2.f, l1 / 2.f};
  FloatRect textRect = Text(xo_font, "X", big_board_font_size).getLocalBounds();
  Vector2f text_origin_big = textRect.position + textRect.size / 2.f;

  // To center the small texts
  Vector2f txtOff_mini{l2 / 2.f, l2 / 2.f};
  textRect = Text(xo_font, "X", mini_board_font_size).getLocalBounds();
  Vector2f text_origin_mini = textRect.position + textRect.size / 2.f;

  Color bigBoardOutlineCol = Color(200, 200, 200);
  Color miniBoardOutlineCol = Color(120, 120, 120);

  // MAIN BOARD PART
  SetSingleRectGui(outerContainerO, off0, {l0, l0}, m0, {0, 0},
                   Color::Transparent, backgroundColor);
  SetSingleRectGui(outerContainerM, off0, {l0, l0}, -m0, {0, 0},
                   Color::Transparent, backgroundColor);

  // BIG BOARD PART
  SetBoardLayoutGui(*this, bigBoardContainerO, off1, l1, t1,
                    bigBoardOutlineCol);  // for outline
  SetBoardLayoutGui(*this, bigBoardContainerM, off1, l1, -m1,
                    backgroundColor);  // for margin
  SetBoardTextGui(*this, bigBoardText, off1, txtOff_big, text_origin_big, l1,
                  big_board_font_size,
                  Color(200, 200, 200, 200));  // for big texts

  // MINI BOARD PART
  miniBoardContainer.reserve(9);
  for (int i = 0; i < 9; i++) {
    miniBoardContainer.push_back({});
    miniBoardContainer[i].reserve(9);
    Vector2f off2 = off1 + Vector2f((i % 3) * (l1), (i / 3) * (l1)) +
                    Vector2f(m1 - m2, m1 - m2);

    SetBoardLayoutGui(*this, miniBoardContainer[i], off2, l2, -m2,
                      miniBoardOutlineCol);
    SetBoardTextGui(*this, miniBoardText[i], off2, txtOff_mini,
                    text_origin_mini, l2, mini_board_font_size, Color::White);
  }

  // HUD
  SetHudGui(*this, hud_width, hud_font_size);
}

void Game_Gui::UpdateResized() {
  // STORE THINGS THAT NEED TO PERSIST AFTER RESIZE
  vector<Color> bbc_fillcolor(9);                         // For fillcolor (big)
  vector<string> bbt_text(9);                             // for text (big)
  vector<vector<string>> mbt_text(9, vector<string>(9));  // for text (small)
  string wnr_txt;
  bool tgl_slvX, tgl_slvO;

  // Store single unit things here
  wnr_txt = winnerText.getString();
  tgl_slvO = game.toggleSolveO;
  tgl_slvX = game.toggleSolveX;

  for (int i = 0; i < 9;
       i++)  // Store required things using this loop (if applicable)
  {
    bbc_fillcolor[i] = bigBoardContainerM[i].getFillColor();
    bbt_text[i] = bigBoardText[i].getString();
    for (int j = 0; j < 9; j++) {
      mbt_text[i][j] = miniBoardText[i][j].getString();
    }
  }

  ClearVisuals();  // Make every vector to the state prior to first
                   // initialization
  Init(margin, thickness, hud_width);  // RE INITIALIZE THE ENTIRE GUI

  // RE ASSIGN WHAT NEEDED TO PERSIST
  // single units here
  winnerText.setString(wnr_txt);
  game.toggleSolveO = tgl_slvO;
  game.toggleSolveX = tgl_slvX;

  for (int i = 0; i < 9; i++)  // loop applicable here
  {
    bigBoardContainerM[i].setFillColor(bbc_fillcolor[i]);
    bigBoardText[i].setString(bbt_text[i]);

    for (int j = 0; j < 9; j++) {
      miniBoardText[i][j].setString(mbt_text[i][j]);
    }
  }
}

void Game_Gui::HighlightBestMove() {
  static Index prev_bestx = {-1, -1};
  static Index prev_besto = {-1, -1};

  // Remove the previously highlighted block and highligh new block
  if (game.currentTurn == 1 &&
      game.toggleSolveX)  // X turn so don't highlight O related stuff
  {
    if (prev_besto.i != -1 && prev_besto.j != -1) {
      miniBoardContainer[prev_besto.i][prev_besto.j].setFillColor(
          Color::Transparent);
      prev_besto = {-1, -1};
    }

    if (game.bestMoveX.i != -1 && game.bestMoveX.j != -1) {
      miniBoardContainer[game.bestMoveX.i][game.bestMoveX.j].setFillColor(
          Color(0, 100, 0, 180));
      prev_bestx = game.bestMoveX;
    }
  } else if (game.currentTurn == -1 &&
             game.toggleSolveO)  // O turn so don't highlight X related stuff
  {
    if (prev_bestx.i != -1 && prev_bestx.j != -1) {
      miniBoardContainer[prev_bestx.i][prev_bestx.j].setFillColor(
          Color::Transparent);
      prev_bestx = {-1, -1};
    }
    if (game.bestMoveO.i != -1 && game.bestMoveO.j != -1) {
      miniBoardContainer[game.bestMoveO.i][game.bestMoveO.j].setFillColor(
          Color(100, 0, 0, 180));
      prev_besto = game.bestMoveO;
    }
  }
}

void Game_Gui::HoverHandle(const Event::MouseMoved *moved) {
  Vector2f cursorPos = Vector2f(moved->position);
  static int prev_i = 0;  // previously hovered big board element
  static int prev_j = 0;  // previously hovered mini board element

  // HOVERING EFFECT FOR RESTART BUTTON
  if (restartButton.getGlobalBounds().contains(cursorPos))
    restartButton.setFillColor(Color(100, 100, 100, 220));
  else
    restartButton.setFillColor(Color(100, 100, 100, 140));

  // HOVERING EFFECT FOR SOLVE O BUTTON
  if (solveOButton.getGlobalBounds().contains(cursorPos))
    solveOButton.setFillColor(Color(100, 100, 100, 220));
  else if (game.toggleSolveO)
    solveOButton.setFillColor(Color(100, 0, 0, 200));
  else
    solveOButton.setFillColor(Color(100, 100, 100, 140));

  // HOVERING EFFECT FOR SOLVE X BUTTON
  if (solveXButton.getGlobalBounds().contains(cursorPos))
    solveXButton.setFillColor(Color(100, 100, 100, 220));
  else if (game.toggleSolveX)
    solveXButton.setFillColor(Color(0, 100, 0, 200));
  else
    solveXButton.setFillColor(Color(100, 100, 100, 140));

  // hovering effect removed from previous element if hovering new element
  if (!miniBoardContainer[prev_i][prev_j].getGlobalBounds().contains(cursorPos))
    miniBoardContainer[prev_i][prev_j].setFillColor(Color::Transparent);

  // HOVERING EFFECT FOR BOARD ELEMENTS
  if (outerContainerM.getGlobalBounds().contains(cursorPos)) {
    int i = 0;
    for (auto &bbc : bigBoardContainerM) {
      if (bbc.getGlobalBounds().contains(cursorPos) &&
          game.gameBoard.winners[i] == 0) {
        prev_i = i;
        int j = 0;
        for (auto &mbc : miniBoardContainer[i]) {
          if (mbc.getGlobalBounds().contains(cursorPos)) {
            prev_j = j;
            miniBoardContainer[i][j].setFillColor(Color(80, 80, 80, 80));
            break;
          }
          j++;
        }
        break;
      }
      i++;
    }
  }
  if (game.toggleSolveO || game.toggleSolveX) {
    HighlightBestMove();
  }
}

void Game_Gui::ClickHandle(const Event::MouseButtonReleased *clicked) {
  Vector2f cursorPos = Vector2f(clicked->position);

  if (restartButton.getGlobalBounds().contains(cursorPos)) {
    game.Restart();  // game restart
    Restart();       // gui restart
    prevPlayableIndex = -1;
    return;
  }

  if (solveOButton.getGlobalBounds().contains(cursorPos)) {
    game.toggleSolveO = game.toggleSolveO ? false : true;
    solveOButton.setFillColor(game.toggleSolveO ? Color(100, 0, 0, 200)
                                                : Color(100, 100, 100, 140));
    return;
  }

  if (solveXButton.getGlobalBounds().contains(cursorPos)) {
    game.toggleSolveX = game.toggleSolveX ? false : true;
    solveXButton.setFillColor(game.toggleSolveX ? Color(0, 100, 0, 200)
                                                : Color(100, 100, 100, 140));
    return;
  }

  if (!game.running)  // when game is stopped clicking doesn't work (except
                      // restart button)
    return;

  if (outerContainerM.getGlobalBounds().contains(cursorPos)) {
    int i = 0;
    for (auto &bbc : bigBoardContainerM) {
      if (bbc.getGlobalBounds().contains(cursorPos)) {
        int j = 0;
        for (auto &mbc : miniBoardContainer[i]) {
          if (mbc.getGlobalBounds().contains(cursorPos) &&
              game.gameBoard.miniBoards[i].board[j] == 0) {
            if (game.CanPlayOn(i, j)) {
              miniBoardText[i][j].setString(game.currentTurn == 1 ? "X" : "O");
              game.UpdatesOnClick(i,
                                  j);  // Game's actual main logic starts here
              UpdateGuiOnClick(
                  i, j, prevPlayableIndex);  // Updates gui that needs to be
                                             // updated after valid clicking

              // Run Solver
              if (game.toggleSolveX && game.currentTurn == 1) {
                BigBoard temp = game.gameBoard;
                game.bestMoveX = game.solver.FindBestMove(
                    temp, game.playableIndex, game.currentTurn,
                    game.solverDepth, game.playedTotalTurns);
              }

              if (game.toggleSolveO && game.currentTurn == -1) {
                BigBoard temp = game.gameBoard;
                game.bestMoveO = game.solver.FindBestMove(
                    temp, game.playableIndex, game.currentTurn,
                    game.solverDepth, game.playedTotalTurns);
              }
              HighlightBestMove();
            }

            return;
          }

          j++;
        }
        return;
      }
      i++;
    }
  }
}

void Game_Gui::UpdateGuiOnClick(int i, int j, int &prevPlayableIndex) {
  // Highlight playable miniboard

  
  if (game.playableIndex != -1) {
    if (prevPlayableIndex != -1)
      bigBoardContainerM[prevPlayableIndex].setFillColor(Color::Transparent);
    bigBoardContainerM[game.playableIndex].setFillColor(
        Color(100, 100, 100, 50));
    prevPlayableIndex = game.playableIndex;
  } else {
    if (prevPlayableIndex != -1)
      bigBoardContainerM[prevPlayableIndex].setFillColor(Color::Transparent);
    prevPlayableIndex = -1;
  }

  // Update non-playable boards (if winner or drawed)
  if (game.gameBoard.winners[i] != 0) {
    char result = game.gameBoard.winners[i] == 1
                      ? 'X'
                      : (game.gameBoard.winners[i] == -1 ? 'O' : ' ');
    bigBoardText[i].setString(result);
    bigBoardContainerM[i].setFillColor(Color(0, 0, 0, 200));
  }

  // Update HUD
  string trntxt = "Player Turn : " + string(game.currentTurn == 1 ? "X" : "O");
  playerTurnText.setString(trntxt);

  if (game.winner != 0) {
    string result = game.winner == 1
                        ? "X" : (game.winner == -1 ? "O" : "Draw");
    string wnrTxt = "Winner : " + result;
    winnerText.setString(wnrTxt);
  }
}

void Game_Gui::Restart() {
  ClearVisuals();

  // Initialize the gui again
  Init(margin, thickness, hud_width);
}

void Game_Gui::ClearVisuals() {
  // Clear all vectors
  bigBoardContainerM.clear();
  bigBoardContainerO.clear();
  miniBoardContainer.clear();
  miniBoardText.clear();
  bigBoardText.clear();

  miniBoardText.resize(9, vector<Text>(9, xo_font));
  bigBoardText.resize(9, xo_font);
}

void Game_Gui::DrawVisuals() {
  // X and O text inside smallest box
  for (auto &mbt : miniBoardText)
    for (auto &t : mbt) window.draw(t);

  // smallest boxes
  for (auto &mbs : miniBoardContainer)
    for (auto &mb : mbs) window.draw(mb);

  // big boxes (margin)
  for (auto &bb : bigBoardContainerM) window.draw(bb);

  // big boxes (outline)
  for (auto &bb : bigBoardContainerO) window.draw(bb);

  // largest box (margin and outline)
  window.draw(outerContainerM);
  window.draw(outerContainerO);

  // X and O text inside one tictactoe (winner of each board)
  for (auto &bbt : bigBoardText) window.draw(bbt);

  // Hud part
  window.draw(restartButton);
  window.draw(restartText);
  window.draw(solveOButton);
  window.draw(solveOText);
  window.draw(solveXButton);
  window.draw(solveXText);
  window.draw(playerTurnText);

  if (game.winner != 0) window.draw(winnerText);
}
