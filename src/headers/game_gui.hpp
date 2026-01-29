#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "game.hpp"

using namespace sf;

class Game_Gui
{
public:
    // References
    RenderWindow &window;
    Game &game;

    // Fonts
    Font txt_font; // for noraml texts
    Font xo_font;  // for X and O

    // Playing Area Related
    int mainContainerSize;
    int margin;
    int thickness;
    Color backgroundColor;
    Vector2f offset;

    // Drawing Shapes
    RectangleShape outerContainerM;
    RectangleShape outerContainerO;
    vector<RectangleShape> bigBoardContainerO;
    vector<RectangleShape> bigBoardContainerM;
    vector<vector<RectangleShape>> miniBoardContainer;
    vector<vector<Text>> miniBoardText;
    vector<Text> bigBoardText;

    // HUD Related
    int hud_width;
    RectangleShape upperHudContainer;
    RectangleShape lowerHudContainer;
    Text playerTurnText;          // Shown in upper hud
    Text restartText;             // Shown in upper hud
    Text winnerText;              // shown in lower hud
    RectangleShape restartButton; // Shown in upper hud

    Game_Gui(Game &g, RenderWindow &w);

    void Init(int mrgns, int thkns, int hudWdth);
    void UpdateResized();
    void HoverHandle(const Event::MouseMoved *moved);
    void ClickHandle(const Event::MouseButtonReleased *clicked);
    void UpdateGuiOnClick(int i, int j, int &prevPlayableIndex);
    void Restart();
    void ClearVisuals();
    void DrawVisuals();
};
