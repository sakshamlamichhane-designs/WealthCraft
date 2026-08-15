#include "Game.h"
#include "GameWindow.h"

int main()
{
    Game game;

    game.initialize();

    GameWindow window(game);

    window.run();

    game.shutdown();

    return 0;
}