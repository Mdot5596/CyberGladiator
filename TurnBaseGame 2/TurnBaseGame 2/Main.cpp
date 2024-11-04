#include "Game.h"
#include "Player.h"
#include "Enemy.h"

int main(int argc, char* argv[])
{

    Game game;
    game.init("Cyber Gladiator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false); //mayb change?
    // Game Loop
    while (game.running())
    {
        game.handleEvents();
        game.update();
        game.render();
    }
    //Clean uP
    game.clean();
    return 0;
}