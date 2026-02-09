#pragma once
#include "gui/game_gui.hpp"
#include "solver/minimax.hpp"

// Rect, text and Layout handling (gui)

void SetSingleRectGui(RectangleShape &rct, Vector2f pos, Vector2f size,
                      float outline_thickness = 0, Vector2f origin = {0, 0},
                      Color rect_color = Color::White,
                      Color outline_color = Color::White);

void SetSingleTextGui(Text &txt, string txt_content, Vector2f pos,
                      float font_size, Color text_color = Color::White);

void SetBoardLayoutGui(Game_Gui &gui, vector<RectangleShape> &board,
                       Vector2f off, float l, float m, Color outline_col);

void SetBoardTextGui(Game_Gui &gui, vector<Text> &txts, Vector2f off,
                     Vector2f txt_off, Vector2f origin, float l,
                     float font_size, Color text_color);

void SetHudGui(Game_Gui &gui, float hud_width, float hud_font_size);

