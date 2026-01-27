#include "game.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>

using namespace sf;

Font font("res/minasans-font/font.otf");

class Game_Gui
{
public:
    // Window
    RenderWindow &window;

    // Playing Area Related
    int mainContainerSize;
    int margin;
    int thickness;
    Color backgroundColor;
    Vector2f offset;

    Game &game;
    RectangleShape outerContainerM;
    RectangleShape outerContainerO;
    vector<RectangleShape> bigBoardContainerO;
    vector<RectangleShape> bigBoardContainerM;
    vector<vector<RectangleShape>> miniBoardContainer;
    vector<vector<Text>> miniBoardText;

    // HUD Related
    int hud_width;
    RectangleShape upperHudContainer;
    RectangleShape lowerHudContainer;
    Text playerTurnText;          // Shown in upper hud
    Text restartText;             // Shown in upper hud
    Text winnerText;              // shown in lower hud
    RectangleShape restartButton; // Shown in upper hud

    Game_Gui(Game &g, RenderWindow &w) : game(g), window(w), backgroundColor(Color::Black), offset({0, 0}), miniBoardText(9, vector<Text>(9, font)), playerTurnText(font), restartText(font), winnerText(font), hud_width(100)
    {
    }

    void Init(Vector2f &windowSize, int mcs, int mrgns, int thkns, int hudWdth);
    void HoverHandle(const Event::MouseMoved *moved);
    void ClickHandle(const Event::MouseButtonReleased *clicked);
    void Restart();
};

void Game_Gui::Init(Vector2f &windowSize, int mcs, int mrgns, int thkns, int hudWdth)
{
    mainContainerSize = mcs;
    margin = mrgns;
    thickness = thkns;
    hud_width = hudWdth;

    float l0 = mcs;
    float m0 = margin;
    float t0 = thickness;

    Color bigBoardOutlineCol = Color(200, 200, 200);
    Color miniBoardOutlineCol = Color(120, 120, 120);

    Vector2f off0{(windowSize.x - l0) / 2.f, (windowSize.y - l0) / 2.f};
    offset = off0;

    // Outer Bound (for Outline)
    outerContainerO = RectangleShape({l0, l0});
    outerContainerO.setPosition(off0);
    outerContainerO.setFillColor(Color::Transparent);
    outerContainerO.setOutlineThickness(m0);
    outerContainerO.setOutlineColor(backgroundColor);

    // Outer Bound (for margin)
    outerContainerM = RectangleShape({l0, l0});
    outerContainerM.setPosition(off0);
    outerContainerM.setFillColor(Color::Transparent);
    outerContainerM.setOutlineThickness(-m0);
    outerContainerM.setOutlineColor(backgroundColor);

    float m1 = 3.5f * m0;
    float l1 = l0 / 3.f;
    float t1 = thickness;
    Vector2f off1 = off0;

    // Big Board (for outline)
    bigBoardContainerO.clear();
    bigBoardContainerO.reserve(9);
    for (int i = 0; i < 9; i++)
    {
        bigBoardContainerO.emplace_back();
        bigBoardContainerO[i].setSize({l1, l1});
        bigBoardContainerO[i].setFillColor(Color::Transparent);
        bigBoardContainerO[i].setOutlineThickness(t1);
        bigBoardContainerO[i].setOutlineColor(bigBoardOutlineCol);

        bigBoardContainerO[i].setPosition(off1 + Vector2f((i % 3) * (l1), (i / 3) * (l1)));
    }
    // Big Board (for margin)
    bigBoardContainerM.clear();
    bigBoardContainerM.reserve(9);
    for (int i = 0; i < 9; i++)
    {
        bigBoardContainerM.emplace_back();
        bigBoardContainerM[i].setSize({l1, l1});
        bigBoardContainerM[i].setFillColor(Color::Transparent);
        bigBoardContainerM[i].setOutlineThickness(-m1);
        bigBoardContainerM[i].setOutlineColor(backgroundColor);

        bigBoardContainerM[i].setPosition(off1 + Vector2f((i % 3) * (l1), (i / 3) * (l1)));
    }

    float m2 = 1.f;
    float l2 = (l1 - 2.f * m1 + 2 * m2) / 3.f;

    // Mini Boards
    miniBoardContainer.clear();
    miniBoardContainer.reserve(9);
    for (int i = 0; i < 9; i++)
    {
        Vector2f off2 = off1 + Vector2f((i % 3) * (l1), (i / 3) * (l1)) + Vector2f(m1 - m2, m1 - m2);

        miniBoardContainer.push_back({});
        miniBoardContainer[i].reserve(9);

        vector<RectangleShape> &mbc = miniBoardContainer[i];
        vector<Text> &mbt = miniBoardText[i];

        int font_size = (0.8f * l2);
        for (int j = 0; j < 9; j++)
        {
            // Smallest Box
            mbc.emplace_back();
            mbc[j].setSize({l2, l2});
            mbc[j].setFillColor(Color::Transparent);
            mbc[j].setOutlineThickness(-m2);
            mbc[j].setOutlineColor(miniBoardOutlineCol);
            mbc[j].setPosition(off2 + Vector2f((j % 3) * (l2), (j / 3) * (l2)));

            // Text Inside Box
            Vector2f txtOff{(l2 - font_size) / 2.f, (l2 - font_size - font_size / 4.f) / 2.f};
            mbt[j].setCharacterSize(font_size);
            mbt[j].setPosition(off2 + Vector2f((j % 3) * (l2), (j / 3) * (l2)) + txtOff);
        }
    }

    // HUD
    // Upper hud
    int hud_font_size = 36;
    upperHudContainer.setSize({l0, (float)hud_width});
    upperHudContainer.setFillColor(backgroundColor);
    upperHudContainer.setPosition({off0.x, off0.y - hud_width});

    // Player turn text
    playerTurnText.setCharacterSize(hud_font_size);
    string trntxt = "Player Turn : " + string(game.currentTurn == 1 ? "X" : "O");
    playerTurnText.setString(trntxt);
    FloatRect textRect = playerTurnText.getLocalBounds();
    playerTurnText.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                              textRect.position.y + textRect.size.y / 2.0f});
    playerTurnText.setPosition({off0.x + mainContainerSize * 0.80f, off0.y / 2.f});

    // restart button with text
    restartText.setCharacterSize(hud_font_size);
    string rstTxt = "Restart";
    restartText.setString(rstTxt);
    textRect = restartText.getLocalBounds();
    restartText.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                           textRect.position.y + textRect.size.y / 2.0f});
    restartText.setPosition({off0.x + mainContainerSize * 0.20f, off0.y / 2.f});
    restartButton.setOrigin(restartText.getOrigin());
    restartButton.setSize(restartText.getLocalBounds().size + Vector2f(hud_font_size / 2.f, hud_font_size / 2.f));
    restartButton.setPosition(restartText.getPosition() - Vector2f(hud_font_size / 4.f, 0.f));
    restartButton.setFillColor(Color(100, 100, 100, 140));

    // Lower HUD
    lowerHudContainer.setSize({l0, (float)hud_width});
    lowerHudContainer.setFillColor(backgroundColor);
    upperHudContainer.setPosition({off0.x, off0.y + mainContainerSize + hud_width});

    // winner text
    winnerText.setCharacterSize(hud_font_size);
    string wnrTxt = "Winner : " + string(game.winner == 1 ? "X" : "O");
    winnerText.setString(wnrTxt);
    textRect = winnerText.getLocalBounds();
    winnerText.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                          textRect.position.y + textRect.size.y / 2.0f});
    winnerText.setPosition({off0.x + mainContainerSize * 0.5f, off0.y + mainContainerSize + hud_width / 2.f});
}

void Game_Gui::HoverHandle(const Event::MouseMoved *moved)
{

    Vector2f cursorPos = Vector2f(moved->position);
    static int prev_i = 0;
    static int prev_j = 0;

    // hovering effect for restart button
    if (restartButton.getGlobalBounds().contains(cursorPos))
        restartButton.setFillColor(Color(100, 100, 100, 220));
    else
        restartButton.setFillColor(Color(100, 100, 100, 140));

    // hovering effect for board elements
    if (!miniBoardContainer[prev_i][prev_j].getGlobalBounds().contains(cursorPos))
        miniBoardContainer[prev_i][prev_j].setFillColor(Color::Transparent);

    if (outerContainerM.getGlobalBounds().contains(cursorPos))
    {
        int i = 0;
        for (auto &bbc : bigBoardContainerM)
        {
            if (bbc.getGlobalBounds().contains(cursorPos))
            {
                prev_i = i;
                int j = 0;
                for (auto &mbc : miniBoardContainer[i])
                {
                    if (mbc.getGlobalBounds().contains(cursorPos))
                    {
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
}

void Game_Gui::ClickHandle(const Event::MouseButtonReleased *clicked)
{
    Vector2f cursorPos = Vector2f(clicked->position);
    static int prevPlayableIndex = -1;

    if (restartButton.getGlobalBounds().contains(cursorPos))
    {
        game.Restart(); // game restart
        Restart();      // gui restart
        prevPlayableIndex = -1;
        return;
    }

    if (!game.running)
        return;
    
    if (outerContainerM.getGlobalBounds().contains(cursorPos))
    {
        int i = 0;
        for (auto &bbc : bigBoardContainerM)
        {
            if (bbc.getGlobalBounds().contains(cursorPos))
            {
                int j = 0;
                for (auto &mbc : miniBoardContainer[i])
                {
                    if (mbc.getGlobalBounds().contains(cursorPos) && game.gameBoard.miniBoards[i].board[j] == 0)
                    {
                        if (game.CanPlayOn(i, j, prevPlayableIndex))
                        {
                            miniBoardText[i][j].setString(game.currentTurn == 1 ? "X" : "O");

                            game.UpdatesOnClick(i, j);
                            // Highlight playable miniboard
                            if (game.playableIndex != -1)
                            {
                                if (prevPlayableIndex != -1)
                                    bigBoardContainerM[prevPlayableIndex].setFillColor(Color::Transparent);
                                bigBoardContainerM[game.playableIndex].setFillColor(Color(100, 100, 100, 50));
                                prevPlayableIndex = game.playableIndex;
                            } else {
                                if(prevPlayableIndex != -1)
                                    bigBoardContainerM[prevPlayableIndex].setFillColor(Color::Transparent);
                                prevPlayableIndex = -1;

                            }

                            // Update HUD
                            string trntxt = "Player Turn : " + string(game.currentTurn == 1 ? "X" : "O");
                            playerTurnText.setString(trntxt);

                            if (game.winner != 0)
                            {
                                string wnrTxt = "Winner : " + string(game.winner == 1 ? "X" : "O");
                                winnerText.setString(wnrTxt);
                            }
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

void Game_Gui::Restart()
{

    Vector2f wndSize = Vector2f(window.getSize());

    // Clear all vectors
    bigBoardContainerM.clear();
    bigBoardContainerO.clear();
    miniBoardContainer.clear();
    miniBoardText.clear();

    miniBoardText.resize(9, vector<Text>(9, font));
    // Initialize the gui again
    Init(wndSize, mainContainerSize, margin, thickness, hud_width);
}

void DrawVisuals(Game_Gui &gui)
{
    // smallest boxes
    for (auto &mbs : gui.miniBoardContainer)
        for (auto &mb : mbs)
            gui.window.draw(mb);

    // big boxes (margin)
    for (auto &bb : gui.bigBoardContainerM)
        gui.window.draw(bb);

    // big boxes (outline)
    for (auto &bb : gui.bigBoardContainerO)
        gui.window.draw(bb);

    // largest box (margin and outline)
    gui.window.draw(gui.outerContainerM);
    gui.window.draw(gui.outerContainerO);

    // X and O text
    for (auto &mbt : gui.miniBoardText)
        for (auto &t : mbt)
            gui.window.draw(t);

    // Hud part
    gui.window.draw(gui.restartButton);
    gui.window.draw(gui.restartText);

    gui.window.draw(gui.playerTurnText);

    if (gui.game.winner != 0)
        gui.window.draw(gui.winnerText);
}
