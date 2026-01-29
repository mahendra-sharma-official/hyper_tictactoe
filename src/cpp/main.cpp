#include "game_gui.hpp"

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Hyper TicTacToe");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    // icon
    sf::Image icon_img;
    if (!icon_img.loadFromFile("res/icon/logo.png"))
        return -1;

    window.setIcon(icon_img.getSize(), icon_img.getPixelsPtr());
    // Game Initialization
    Game game;
    game.Init();

    Game_Gui gui(game, window);
    int hud_width = 100;
    int margins = 10;
    int thickness = 2;

    gui.Init(margins, thickness, hud_width);

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

                gui.UpdateResized();
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
        }

        // Logic
        if (game.running)
        {
        }
        // Background
        window.clear(sf::Color::Black);

        // Draw calls here
        gui.DrawVisuals();

        // end the current frame
        window.display();
    }
    return 0;
}