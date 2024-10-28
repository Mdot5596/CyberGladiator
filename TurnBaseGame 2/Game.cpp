#include "Game.h"
#include <iostream>
#include "TextureManager.h"


Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), currentState(GameState::MENU) {}


Game::~Game() 
{
    clean();
}



void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) 
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
    {
        window = SDL_CreateWindow(title, xpos, ypos, 1024, 768, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
        renderer = SDL_CreateRenderer(window, -1, 0);
        isRunning = (window && renderer);
        std::cout << "SDL initialized successfully.\n";


        //Load Textures below
        menuBackgroundTexture = TextureManager::loadTexture("assets/background1.png", renderer);
        buttonNormalTexture = TextureManager::loadTexture("assets/STARTBTN.png", renderer);
        buttonHoverTexture = TextureManager::loadTexture("assets/startbown.png", renderer);

        backgroundTexture = TextureManager::loadTexture("assets/background.png", renderer);
        gladiatorTexture = TextureManager::loadTexture("assets/dude.png", renderer);


       
    }
    else 
    {
        isRunning = false;
    }
}



void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_QUIT) 
        {
            isRunning = false;
        }

        else if (event.type == SDL_MOUSEMOTION) 
        {
            // Check if the mouse is hovering over the button
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;
            if (mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w &&
                mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h) 
            {
                startButtonState = HOVER;
            }
            else 
            {
                startButtonState = NORMAL;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN && startButtonState == HOVER) 
        {
            currentState = GameState::PLAY;
        }
    }
}



void Game::update() 
{
    // Game logic will be added here later.
   


}



void Game::render() 
{
    //menu Start
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);  // Set background color
    SDL_RenderClear(renderer);

    if (currentState == GameState::MENU) 
    {
        renderMenu();
    }
    else if (currentState == GameState::PLAY) 
    {
        renderPlay();
    }

    SDL_RenderPresent(renderer);  // Present the current frame
    //menu End


    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);  // BackGround Colour
    SDL_RenderClear(renderer);



    //Background Render 1
    SDL_Rect destRect = { 0, 0, 1024, 768 };   // Position and size
    TextureManager::render(backgroundTexture, renderer, destRect);


    //Main character Render 2nd
    SDL_Rect gladiatorRect = { 100, 100, 150, 150 };  // Position and size
    TextureManager::render(gladiatorTexture, renderer, gladiatorRect);
    

    
}


//Menu Funcs

void Game::renderMenu() 
{

    // Render background
    SDL_Rect destRect = { 0, 0, 1024, 768 };  // Adjust to your window size
    TextureManager::render(menuBackgroundTexture, renderer, destRect);

    // Choose the texture for the button based on its state
    SDL_Texture* buttonTexture = nullptr;
    if (startButtonState == NORMAL) 
    {
        buttonTexture = buttonNormalTexture;
    }
    else if (startButtonState == HOVER)
    {
        buttonTexture = buttonHoverTexture;
    }
    else if (startButtonState == PRESSED) 
    {
        buttonTexture = buttonPressedTexture;
    }

    // Render the button
    TextureManager::render(buttonTexture, renderer, startButtonRect);
}

void Game::renderPlay() 
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);  // Background color
    SDL_RenderClear(renderer);



    // Render background and gladiator
    SDL_Rect destRect = { 0, 0, 1024, 768 };  // Background position and size
    TextureManager::render(backgroundTexture, renderer, destRect);

    SDL_Rect gladiatorRect = { 100, 100, 150, 150 };  // Gladiator position and size
    TextureManager::render(gladiatorTexture, renderer, gladiatorRect);

    SDL_RenderPresent(renderer);
}

//Menu End


void Game::clean() 
{
    TextureManager::clean(backgroundTexture);
    TextureManager::clean(gladiatorTexture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}



bool Game::running() const 
{
    return isRunning;
}
