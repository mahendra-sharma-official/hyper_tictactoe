#include "game_gui.hpp"

Game_Gui::Game_Gui(Game &g, RenderWindow &w)
    : game(g),
      window(w),
      txt_font("res/fonts/craftycandy.otf"),
      xo_font("res/fonts/craftycandy.otf"),
      playerTurnText(txt_font),
      restartText(txt_font),
      winnerText(txt_font)
{
    hud_width = 100;
    backgroundColor = Color::Black;
    miniBoardText.assign(9, vector<Text>(9, xo_font));
    bigBoardText.assign(9, xo_font);
}

// Helpers for creating game gui elements
void SetSingleRectGui(RectangleShape &rct, Vector2f pos, Vector2f size, float outline_thickness = 0, Vector2f origin = {0, 0}, Color rect_color = Color::White, Color outline_color = Color::White)
{
    rct.setOrigin(origin);
    rct.setSize(size);
    rct.setOutlineThickness(outline_thickness);
    rct.setPosition(pos);
    rct.setFillColor(rect_color);
    rct.setOutlineColor(outline_color);
}

void SetSingleTextGui(Text &txt, string txt_content, Vector2f pos, float font_size, Color text_color = Color::White)
{
    txt.setCharacterSize(font_size);
    txt.setString(txt_content);
    FloatRect textRect = txt.getLocalBounds();
    txt.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                   textRect.position.y + textRect.size.y / 2.0f});
    txt.setPosition(pos);
    txt.setFillColor(text_color);
}

void SetBoardLayoutGui(Game_Gui &gui, vector<RectangleShape> &board, Vector2f off, float l, float m, Color outline_col)
{
    board.reserve(9);
    for (int i = 0; i < 9; i++)
    {
        board.emplace_back();
        SetSingleRectGui(board[i], off + Vector2f((i % 3) * (l), (i / 3) * (l)), {l, l}, m, {0, 0}, Color::Transparent, outline_col);
    }
}

void SetBoardTextGui(Game_Gui &gui, vector<Text> &txts, Vector2f off, Vector2f txt_off, Vector2f origin, float l, float font_size, Color text_color)
{
    txts.reserve(9);
    for (int i = 0; i < 9; i++)
    {
        SetSingleTextGui(txts[i], " ", off + Vector2f((i % 3) * (l), (i / 3) * (l)) + txt_off, font_size, text_color);
        txts[i].setOrigin(origin);
        txts[i].setStyle(Text::Bold);
    }
}

void SetHudGui(Game_Gui &gui, float hud_width, float hud_font_size)
{
    // UPPER HUD PART
    SetSingleRectGui(gui.upperHudContainer, gui.offset - Vector2f(0, hud_width), {(float)gui.mainContainerSize, hud_width});
    gui.upperHudContainer.setFillColor(gui.backgroundColor);

    // Player turn text
    string trntxt = "Player Turn : " + string(gui.game.currentTurn == 1 ? "X" : "O");
    SetSingleTextGui(gui.playerTurnText, trntxt, {gui.offset.x + gui.mainContainerSize * 0.80f, gui.offset.y / 2.f}, hud_font_size, Color::White);

    // Restart button with text
    SetSingleTextGui(gui.restartText, "Restart", {gui.offset.x + gui.mainContainerSize * 0.20f, gui.offset.y / 2.f}, hud_font_size, Color::White);
    SetSingleRectGui(gui.restartButton, gui.restartText.getPosition() - Vector2f(hud_font_size / 4.f, 0.f),
                     gui.restartText.getLocalBounds().size + Vector2f(hud_font_size / 2.f, hud_font_size / 2.f), 0, gui.restartText.getOrigin(), Color(130, 130, 130, 130));

    // LOWER HUD PART
    SetSingleRectGui(gui.lowerHudContainer, {gui.offset.x, gui.offset.y + gui.mainContainerSize + hud_width}, {(float)gui.mainContainerSize, hud_width});
    gui.lowerHudContainer.setFillColor(gui.backgroundColor);

    // winner text
    string wnrTxt = "Winner : " + string(gui.game.winner == 1 ? "X" : "O");
    SetSingleTextGui(gui.winnerText, wnrTxt, {gui.offset.x + gui.mainContainerSize * 0.5f, gui.offset.y + gui.mainContainerSize + hud_width / 2.f}, hud_font_size, Color::White);
}

void Game_Gui::Init(int mrgns, int thkns, int hudWdth)
{
    // VALUES NEEDED TO INITIALIZE THE GAME BOARD
    mainContainerSize = window.getSize().y < window.getSize().x ? window.getSize().y : window.getSize().x;
    mainContainerSize -= (hudWdth * 2 + mrgns * 2);
    margin = mrgns;
    thickness = thkns;
    hud_width = hudWdth;

    // For Main board
    float l0 = mainContainerSize;
    float m0 = margin;
    float t0 = thickness;
    Vector2f off0{(window.getSize().x - l0) / 2.f, (window.getSize().y - l0) / 2.f};
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
    SetSingleRectGui(outerContainerO, off0, {l0, l0}, m0, {0, 0}, Color::Transparent, backgroundColor);
    SetSingleRectGui(outerContainerM, off0, {l0, l0}, -m0, {0, 0}, Color::Transparent, backgroundColor);

    // BIG BOARD PART
    SetBoardLayoutGui(*this, bigBoardContainerO, off1, l1, t1, bigBoardOutlineCol);                                              // for outline
    SetBoardLayoutGui(*this, bigBoardContainerM, off1, l1, -m1, backgroundColor);                                                // for margin
    SetBoardTextGui(*this, bigBoardText, off1, txtOff_big, text_origin_big, l1, big_board_font_size, Color(200, 200, 200, 200)); // for big texts

    // MINI BOARD PART
    miniBoardContainer.reserve(9);
    for (int i = 0; i < 9; i++)
    {
        miniBoardContainer.push_back({});
        miniBoardContainer[i].reserve(9);
        Vector2f off2 = off1 + Vector2f((i % 3) * (l1), (i / 3) * (l1)) + Vector2f(m1 - m2, m1 - m2);

        SetBoardLayoutGui(*this, miniBoardContainer[i], off2, l2, -m2, miniBoardOutlineCol);
        SetBoardTextGui(*this, miniBoardText[i], off2, txtOff_mini, text_origin_mini, l2, mini_board_font_size, Color::White);
    }

    // HUD
    SetHudGui(*this, hud_width, hud_font_size);
}

void Game_Gui::UpdateResized()
{
    // STORE THINGS THAT NEED TO PERSIST AFTER RESIZE
    vector<Color> bbc_fillcolor(9);                        // For fillcolor (big)
    vector<string> bbt_text(9);                            // for text (big)
    vector<vector<string>> mbt_text(9, vector<string>(9)); // for text (small)
    string wnr_txt;

    // Store single unit things here
    wnr_txt = winnerText.getString();

    for (int i = 0; i < 9; i++) // Store required things using this loop (if applicable)
    {
        bbc_fillcolor[i] = bigBoardContainerM[i].getFillColor();
        bbt_text[i] = bigBoardText[i].getString();
        for (int j = 0; j < 9; j++)
        {
            mbt_text[i][j] = miniBoardText[i][j].getString();
        }
    }

    ClearVisuals();                     // Make every vector to the state prior to first initialization
    Init(margin, thickness, hud_width); // RE INITIALIZE THE ENTIRE GUI

    // RE ASSIGN WHAT NEEDED TO PERSIST
    // single units here
    winnerText.setString(wnr_txt);

    for (int i = 0; i < 9; i++) // loop applicable here
    {
        bigBoardContainerM[i].setFillColor(bbc_fillcolor[i]);
        bigBoardText[i].setString(bbt_text[i]);

        for (int j = 0; j < 9; j++)
        {
            miniBoardText[i][j].setString(mbt_text[i][j]);
        }
    }
}

void Game_Gui::HoverHandle(const Event::MouseMoved *moved)
{
    Vector2f cursorPos = Vector2f(moved->position);
    static int prev_i = 0; // previously hovered big board element
    static int prev_j = 0; // previously hovered mini board element

    // HOVERING EFFECT FOR RESTART BUTTON
    if (restartButton.getGlobalBounds().contains(cursorPos))
        restartButton.setFillColor(Color(100, 100, 100, 220));
    else
        restartButton.setFillColor(Color(100, 100, 100, 140));

    // hovering effect removed from previous element if hovering new element
    if (!miniBoardContainer[prev_i][prev_j].getGlobalBounds().contains(cursorPos))
        miniBoardContainer[prev_i][prev_j].setFillColor(Color::Transparent);

    // HOVERING EFFECT FOR BOARD ELEMENTS
    if (outerContainerM.getGlobalBounds().contains(cursorPos))
    {
        int i = 0;
        for (auto &bbc : bigBoardContainerM)
        {
            if (bbc.getGlobalBounds().contains(cursorPos) && game.gameBoard.winners[i] == 0)
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

    if (!game.running) // when game is stopped clicking doesn't work (except restart button)
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
                            game.UpdatesOnClick(i, j);                 // Game's actual main logic starts here
                            UpdateGuiOnClick(i, j, prevPlayableIndex); // Updates gui that needs to be updated after valid clicking
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

inline void Game_Gui::UpdateGuiOnClick(int i, int j, int &prevPlayableIndex)
{
    // Highlight playable miniboard
    if (game.playableIndex != -1)
    {
        if (prevPlayableIndex != -1)
            bigBoardContainerM[prevPlayableIndex].setFillColor(Color::Transparent);
        bigBoardContainerM[game.playableIndex].setFillColor(Color(100, 100, 100, 50));
        prevPlayableIndex = game.playableIndex;
    }
    else
    {
        if (prevPlayableIndex != -1)
            bigBoardContainerM[prevPlayableIndex].setFillColor(Color::Transparent);
        prevPlayableIndex = -1;
    }

    // Update non-playable boards (if winner or drawed)
    if (game.gameBoard.winners[i] != 0)
    {
        char result = game.gameBoard.winners[i] == 1 ? 'X' : (game.gameBoard.winners[i] == -1 ? 'O' : 'D');
        bigBoardText[i].setString(result);
        bigBoardContainerM[i].setFillColor(Color(0, 0, 0, 200));
    }

    // Update HUD
    string trntxt = "Player Turn : " + string(game.currentTurn == 1 ? "X" : "O");
    playerTurnText.setString(trntxt);

    if (game.winner != 0)
    {
        char result = game.gameBoard.winners[i] == 1 ? 'X' : (game.gameBoard.winners[i] == -1 ? 'O' : 'D');
        string wnrTxt = "Winner : " + result;
        winnerText.setString(wnrTxt);
    }
}

void Game_Gui::Restart()
{
    ClearVisuals();

    // Initialize the gui again
    Init(margin, thickness, hud_width);
}

void Game_Gui::ClearVisuals()
{
    // Clear all vectors
    bigBoardContainerM.clear();
    bigBoardContainerO.clear();
    miniBoardContainer.clear();
    miniBoardText.clear();
    bigBoardText.clear();

    miniBoardText.resize(9, vector<Text>(9, xo_font));
    bigBoardText.resize(9, xo_font);
}

void Game_Gui::DrawVisuals()
{
    // X and O text inside smallest box
    for (auto &mbt : miniBoardText)
        for (auto &t : mbt)
            window.draw(t);

    // smallest boxes
    for (auto &mbs : miniBoardContainer)
        for (auto &mb : mbs)
            window.draw(mb);

    // big boxes (margin)
    for (auto &bb : bigBoardContainerM)
        window.draw(bb);

    // big boxes (outline)
    for (auto &bb : bigBoardContainerO)
        window.draw(bb);

    // largest box (margin and outline)
    window.draw(outerContainerM);
    window.draw(outerContainerO);

    // X and O text inside one tictactoe (winner of each board)
    for (auto &bbt : bigBoardText)
        window.draw(bbt);

    // Hud part
    window.draw(restartButton);
    window.draw(restartText);
    window.draw(playerTurnText);

    if (game.winner != 0)
        window.draw(winnerText);
}
