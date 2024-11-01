#include "Game.h"
#include "TextureManager.h"
#include <iostream>

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), currentState(GameState::MENU), player("Cyber Gladiator"), enemy("Goblin") {} //to long lol


Game::~Game() 
{
    clean();
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) 
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
    {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
        renderer = SDL_CreateRenderer(window, -1, 0);
        isRunning = (window && renderer);
        std::cout << "SDL initialized successfully.\n";

        //Load Fonts
        TTF_Init();  
        font = TTF_OpenFont("assets/Caviar_Dreams_Bold.ttf", 24); //CHANGE - FONT SUCKS ATM

        // Load Textures
        menuBackgroundTexture = TextureManager::loadTexture("assets/background1.png", renderer);//SHIT
        buttonNormalTexture = TextureManager::loadTexture("assets/STARTBTN.png", renderer);//GOOD
        buttonHoverTexture = TextureManager::loadTexture("assets/startbown.png", renderer);//GOOD
        backgroundTexture = TextureManager::loadTexture("assets/background.png", renderer);//SHIT
        gladiatorTexture = TextureManager::loadTexture("assets/dude.png", renderer);//SHIT
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
            currentState = GameState::PLAY; // Trans to play state
        }
        else if (event.type == SDL_KEYDOWN) 
        {
            if (currentState == GameState::PLAY) 
            {
                if (event.key.keysym.sym == SDLK_1) 
                {
                    player.selectAttack(0); // Select first attack
                    player.performAttack(enemy); // Perform attack on enemy
                }
                else if (event.key.keysym.sym == SDLK_2) 
                {
                    player.selectAttack(1); // Select second attack
                    player.performAttack(enemy); // Perform attack on enemy
                }
                else if (event.key.keysym.sym == SDLK_3) 
                {
                    player.selectAttack(2); // Select third attack
                    player.performAttack(enemy); // Perform attack on enemy
                }
            }
        }
    }
}

void Game::update() 
{
    // Update game logic
    if (!enemy.isAlive())
    {
        std::cout << enemy.getName() << " has been Killed!\n"; //slain instead of killed?
        // NEED TO ADD MORE LOGIC FOR WINNING LIKEEEE AN ANIMATION OR SM
    }
}

void Game::render() 
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set background colour
    SDL_RenderClear(renderer);

    if (currentState == GameState::MENU) 
    {
        renderMenu();
    }
    else if (currentState == GameState::PLAY) 
    {
        renderPlay();
        // Render the goblin's health after rendering the game elements
        renderHealth(enemy.getHealth());
    }

    SDL_RenderPresent(renderer);
}

void Game::renderMenu()
{
    // Render menu background
    SDL_Rect destRect = { 0, 0, 1024, 768 }; // window size
    TextureManager::render(menuBackgroundTexture, renderer, destRect);

    // Choose the texture for the button based on its state
    SDL_Texture* buttonTexture = buttonNormalTexture; // Default button state
    if (startButtonState == HOVER) 
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
    // Render background and gladiator
    SDL_Rect destRect = { 0, 0, 1024, 768 }; // Background position and size
    TextureManager::render(backgroundTexture, renderer, destRect);

    SDL_Rect gladiatorRect = { 100, 100, 150, 150 }; // Gladiator position and size
    TextureManager::render(gladiatorTexture, renderer, gladiatorRect);
}


void Game::renderHealth(int health) 
{
    // Convert health to string
    std::string healthText = "Goblin Health: " + std::to_string(health);
    SDL_Color textColor = { 255, 0, 0 };  // Red color for the text

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, healthText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // Position for the text
    SDL_Rect textRect;
    textRect.x = 10; // X position
    textRect.y = 10; // Y position
    textRect.w = textSurface->w; // Width of the text
    textRect.h = textSurface->h; // Height of the text

    // Render the text
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    // Cleanup
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void Game::clean()
{
    // Clean up textures and SDL
    TextureManager::clean(backgroundTexture);
    TextureManager::clean(gladiatorTexture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);  // Close the font
    TTF_Quit();  // Quit TTF
    SDL_Quit();
}

bool Game::running() const {
    return isRunning;
}
