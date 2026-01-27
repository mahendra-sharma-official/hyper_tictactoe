#include "game_gui.hpp"

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode({1200, 1000}), "Hyper TicTacToe");
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    // Game Initialization
    Game game;
    game.Init();

    Game_Gui gui(game, window);

    Vector2f wndSize = {(float)window.getSize().x, (float)window.getSize().y};
    float smallerWindow = wndSize.y < wndSize.x ? wndSize.y : wndSize.x;
    int hud_width = 100;
    int mcs = 800;
    int margins = 10;
    int thickness = 2;

    mcs = mcs < smallerWindow - 2 * hud_width ? mcs : smallerWindow - 2 * hud_width;

    gui.Init(wndSize, mcs, margins, thickness, hud_width);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::Resized>())
            {
                // resize my view
                sf::Vector2f resized = sf::Vector2f(event->getIf<sf::Event::Resized>()->size);
                window.setView(sf::View(resized / 2.f, resized));

                wndSize = {(float)window.getSize().x, (float)window.getSize().y};
                gui.Init(wndSize, mcs, margins, thickness, hud_width);
            }
            if (event->is<Event::MouseMoved>())
            {
                auto *moved = event->getIf<sf::Event::MouseMoved>();
                gui.HoverHandle(moved);
            }
            if (event->is<Event::MouseButtonReleased>())
            {
                auto *clicked = event->getIf<sf::Event::MouseButtonReleased>();
                gui.ClickHandle(clicked);
            }

            // Logic
            if (game.running)
            {
                game.Update();
            }

            // Background
            window.clear(sf::Color::Black);

            // Draw calls here
            DrawVisuals(gui);

            // end the current frame
            window.display();
        }
    }
}