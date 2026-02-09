#include "gui/gui_helpers.hpp"

// Helpers for creating game gui elements
void SetSingleRectGui(RectangleShape &rct, Vector2f pos, Vector2f size,
                      float outline_thickness, Vector2f origin,
                      Color rect_color, Color outline_color) {
  rct.setOrigin(origin);
  rct.setSize(size);
  rct.setOutlineThickness(outline_thickness);
  rct.setPosition(pos);
  rct.setFillColor(rect_color);
  rct.setOutlineColor(outline_color);
}

void SetSingleTextGui(Text &txt, string txt_content, Vector2f pos,
                      float font_size, Color text_color) {
  txt.setCharacterSize(font_size);
  txt.setString(txt_content);
  FloatRect textRect = txt.getLocalBounds();
  txt.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                 textRect.position.y + textRect.size.y / 2.0f});
  txt.setPosition(pos);
  txt.setFillColor(text_color);
}

void SetBoardLayoutGui(Game_Gui &gui, vector<RectangleShape> &board,
                       Vector2f off, float l, float m, Color outline_col) {
  board.reserve(9);
  for (int i = 0; i < 9; i++) {
    board.emplace_back();
    SetSingleRectGui(board[i], off + Vector2f((i % 3) * (l), (i / 3) * (l)),
                     {l, l}, m, {0, 0}, Color::Transparent, outline_col);
  }
}

void SetBoardTextGui(Game_Gui &gui, vector<Text> &txts, Vector2f off,
                     Vector2f txt_off, Vector2f origin, float l,
                     float font_size, Color text_color) {
  txts.reserve(9);
  for (int i = 0; i < 9; i++) {
    SetSingleTextGui(txts[i], " ",
                     off + Vector2f((i % 3) * (l), (i / 3) * (l)) + txt_off,
                     font_size, text_color);
    txts[i].setOrigin(origin);
    txts[i].setStyle(Text::Bold);
  }
}

void SetHudGui(Game_Gui &gui, float hud_width, float hud_font_size) {
  // UPPER HUD PART
  SetSingleRectGui(gui.upperHudContainer, gui.offset - Vector2f(0, hud_width),
                   {(float)gui.mainContainerSize, hud_width});
  gui.upperHudContainer.setFillColor(gui.backgroundColor);

  // Player turn text
  string trntxt =
      "Player Turn : " + string(gui.game.currentTurn == 1 ? "X" : "O");
  SetSingleTextGui(
      gui.playerTurnText, trntxt,
      {gui.offset.x + gui.mainContainerSize * 0.80f, gui.offset.y / 2.f},
      hud_font_size, Color::White);

  // Restart button with text
  SetSingleTextGui(
      gui.restartText, "Restart",
      {gui.offset.x + gui.mainContainerSize * 0.10f, gui.offset.y / 2.f},
      hud_font_size, Color::White);
  SetSingleRectGui(
      gui.restartButton,
      gui.restartText.getPosition() - Vector2f(hud_font_size / 4.f, 0.f),
      gui.restartText.getLocalBounds().size +
          Vector2f(hud_font_size / 2.f, hud_font_size / 2.f),
      0, gui.restartText.getOrigin(), Color(130, 130, 130, 130));

  // SolveO button with text
  SetSingleTextGui(
      gui.solveOText, "Solve O",
      {gui.offset.x + gui.mainContainerSize * 0.30f, gui.offset.y / 2.f},
      hud_font_size, Color::White);
  SetSingleRectGui(
      gui.solveOButton,
      gui.solveOText.getPosition() - Vector2f(hud_font_size / 4.f, 0.f),
      gui.solveOText.getLocalBounds().size +
          Vector2f(hud_font_size / 2.f, hud_font_size / 2.f),
      0, gui.solveOText.getOrigin(), Color(130, 130, 130, 130));

  // SolveX button with text
  SetSingleTextGui(
      gui.solveXText, "Solve X",
      {gui.offset.x + gui.mainContainerSize * 0.50f, gui.offset.y / 2.f},
      hud_font_size, Color::White);
  SetSingleRectGui(
      gui.solveXButton,
      gui.solveXText.getPosition() - Vector2f(hud_font_size / 4.f, 0.f),
      gui.solveXText.getLocalBounds().size +
          Vector2f(hud_font_size / 2.f, hud_font_size / 2.f),
      0, gui.solveXText.getOrigin(), Color(130, 130, 130, 130));

  // LOWER HUD PART
  SetSingleRectGui(
      gui.lowerHudContainer,
      {gui.offset.x, gui.offset.y + gui.mainContainerSize + hud_width},
      {(float)gui.mainContainerSize, hud_width});
  gui.lowerHudContainer.setFillColor(gui.backgroundColor);

  // Toggle Autoplay button with text
  SetSingleTextGui(gui.autoplayText, "Autoplay",
                   {gui.offset.x + gui.mainContainerSize * 0.25f,
                    gui.offset.y + gui.mainContainerSize + hud_width * 0.55f},
                   hud_font_size, Color::White);
  SetSingleRectGui(
      gui.autoplayButton,
      gui.autoplayText.getPosition() - Vector2f(hud_font_size / 4.f, 0.f),
      gui.autoplayText.getLocalBounds().size +
          Vector2f(hud_font_size / 2.f, hud_font_size * 0.5f),
      0, gui.autoplayText.getOrigin(), Color(130, 130, 130, 130));

  // winner text
  string wnrTxt = "Winner : " + string(gui.game.winner == 1 ? "X" : "O");
  SetSingleTextGui(gui.winnerText, wnrTxt,
                   {gui.offset.x + gui.mainContainerSize * 0.75f,
                    gui.offset.y + gui.mainContainerSize + hud_width * 0.5f},
                   hud_font_size, Color::White);
}