#pragma once

#include <SFML/Graphics.hpp>

#include "Game.h"

class GameWindow
{
public:
    explicit GameWindow(Game& game);

    void run();

private:
    Game& game_;

    sf::RenderWindow window_;
    sf::Clock deltaClock_;

    bool showBuyWindow_;
    bool showSellWindow_;
    bool showLoanWindow_;
    bool showFinanceWindow_;

    double loanAmount_;

    std::string statusMessage_;

    void processEvents();
    void render();

    void drawDashboard();
    void drawBuyWindow();
    void drawSellWindow();
    void drawLoanWindow();
    void drawFinanceWindow();
};