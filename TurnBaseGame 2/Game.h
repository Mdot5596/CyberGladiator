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
    GAME_OVER,
    EXIT

    //WIN AND LOSE NEED 2 B ADDED
};

enum ButtonState { NORMAL, HOVER, PRESSED };

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
    void renderHealth(int health, bool isPlayer);
    void displayAttackOptions();
    void renderAttackButtons();



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
    SDL_Texture* attackButtonTextures[3]; // Textures for each attack button

    //Fonts
    TTF_Font* font;

    //SFX - NEED TO ADD BACKGROUND MUSIC AND SFX

    GameState currentState;
    Player player;                 // Player object
    Enemy enemy;                  // Enemy object

    void renderMenu();
    void renderPlay();
    void renderGameOver();

    //BTN states
    enum ButtonState { NORMAL, HOVER, PRESSED };
    ButtonState startButtonState = NORMAL;
    ButtonState attackButtonStates[3] = { NORMAL, NORMAL, NORMAL };  // Array to store the states of 3 attack buttons

    SDL_Rect startButtonRect = { (1024 - 200) / 2, (768 - 100) / 2, 200, 100 };


    SDL_Rect attackButtonRects[3] =
    {
        { 150, 500, 150, 150 }, // Position for first attack button (x, y, width, height)
        { 350, 500, 150, 150 }, // Position for second attack button
        { 550, 500, 150, 150 }  // Position for third attack button
    };


};