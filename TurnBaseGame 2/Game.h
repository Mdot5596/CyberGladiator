#pragma once
#include <iostream>
//SDL Dependencies
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "TextureManager.h"


enum class GameState 
{
    MENU,
    PLAY,
    EXIT
};


class Game {
public:
    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() const;

private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;

    //Textures add here
    SDL_Texture* backgroundTexture;
    SDL_Texture* gladiatorTexture;
    SDL_Texture* menuBackgroundTexture;
    SDL_Texture* buttonNormalTexture;
    SDL_Texture* buttonHoverTexture;
    SDL_Texture* buttonPressedTexture;


    //menu
    GameState currentState;

    void renderMenu();
    void renderPlay();

    enum ButtonState { NORMAL, HOVER, PRESSED };
    ButtonState startButtonState = NORMAL;

    SDL_Rect startButtonRect = { (1024 - 200) / 2, (768 - 100) / 2, 200, 100 };  // Button's position and size

};

