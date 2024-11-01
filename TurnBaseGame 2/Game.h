#pragma once
#include <iostream>
#include <SDL.h>
#include "Player.h"
#include "Enemy.h"
#include <SDL_ttf.h>


enum class GameState 
{
    MENU,
    PLAY,
    EXIT

    //WIN AND LOSE NEED 2 B ADDED
};

class Game 
{
public:
    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() const;
    void renderHealth(int health);


private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;

    //Textures:
    SDL_Texture* backgroundTexture;
    SDL_Texture* gladiatorTexture;
    SDL_Texture* menuBackgroundTexture;
    SDL_Texture* buttonNormalTexture;
    SDL_Texture* buttonHoverTexture;
    SDL_Texture* buttonPressedTexture;

    //Fonts
    TTF_Font* font;

    //SFX - NEED TO ADD BACKGROUND MUSIC AND SFX

    GameState currentState;
    Player player;                 // Player object
    Enemy enemy;                  // Enemy object

    void renderMenu();
    void renderPlay();

    //BTN states
    enum ButtonState { NORMAL, HOVER, PRESSED };
    ButtonState startButtonState = NORMAL;
    SDL_Rect startButtonRect = { (1024 - 200) / 2, (768 - 100) / 2, 200, 100 };  // Button's position and size

};
