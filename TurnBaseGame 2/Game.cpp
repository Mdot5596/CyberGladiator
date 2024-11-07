#include "Game.h"
#include "TextureManager.h"
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

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

        //Display attack options (terminal) need toSDL This
        displayAttackOptions();

        //Load Fonts
        TTF_Init();
        font = TTF_OpenFont("assets/Caviar_Dreams_Bold.ttf", 24); //CHANGE - FONT SUCKS ATM

        // Load Textures
        menuBackgroundTexture = TextureManager::loadTexture("assets/background1.png", renderer);//SHIT
        buttonNormalTexture = TextureManager::loadTexture("assets/STARTBTN.png", renderer);//GOOD
        buttonHoverTexture = TextureManager::loadTexture("assets/startbown.png", renderer);//GOOD
        backgroundTexture = TextureManager::loadTexture("assets/background.png", renderer);//SHIT
        gladiatorTexture = TextureManager::loadTexture("assets/dude.png", renderer);//SHIT
        attackButtonTextures[0] = TextureManager::loadTexture("assets/ATK BTN 1.png", renderer);
        attackButtonTextures[1] = TextureManager::loadTexture("assets/ATK BTN 1.png", renderer);
        attackButtonTextures[2] = TextureManager::loadTexture("assets/ATK BTN 1.png", renderer);

    }
    else
    {
        isRunning = false;
    }

}


void Game::displayAttackOptions()
{
    std::cout << "Available Attacks" << std::endl;
    const std::vector<std::string>& attackOptions = player.getAttackOptions();
    for (size_t i = 0; i < attackOptions.size(); i++)  //Prints all stored atk options
    {
        std::cout << (i + 1) << "." << attackOptions[i] << std::endl;
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

        // Mouse motion events for hovering over buttons
        else if (event.type == SDL_MOUSEMOTION)
        {
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;

            // Check if hovering over the start button
            if (mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w &&
                mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h)
            {
                startButtonState = HOVER;
            }
            else
            {
                startButtonState = NORMAL;
            }

            // Check if hovering over attack buttons
            for (int i = 0; i < 3; ++i)
            {
                if (mouseX >= attackButtonRects[i].x && mouseX <= attackButtonRects[i].x + attackButtonRects[i].w &&
                    mouseY >= attackButtonRects[i].y && mouseY <= attackButtonRects[i].y + attackButtonRects[i].h)
                {
                    attackButtonStates[i] = HOVER;
                }
                else
                {
                    attackButtonStates[i] = NORMAL;
                }
            }
        }

        // Mouse button down events for clicking buttons
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (startButtonState == HOVER)
            {
                currentState = GameState::PLAY;
            }

            for (int i = 0; i < 3; ++i)
            {
                if (attackButtonStates[i] == HOVER && currentState == GameState::PLAY)
                {
                    player.selectAttack(i);       // Select attack based on button index
                    player.performAttack(enemy);  // Perform the selected attack
                    SDL_Delay(1000);              // Delay to simulate time between attacks

                    // Enemy attacks if still alive
                    if (enemy.isAlive())
                    {
                        std::cout << enemy.getName() << " attacks " << player.getName() << "!\n";
                        player.receiveDamage(5);  // Example damage
                    }

                    // Check for game over condition
                    if (!enemy.isAlive() || !player.isAlive())
                    {
                        currentState = GameState::GAME_OVER;
                    }
                }
            }
        }

        // Restart game with 'R' key
        else if (event.type == SDL_KEYDOWN)
        {
            if (currentState == GameState::GAME_OVER && event.key.keysym.sym == SDLK_r)
            {
                // Reset player and enemy
                player = Player("Cyber Gladiator");
                enemy = Enemy("Goblin");
                currentState = GameState::MENU;
            }
        }
    }
}




void Game::update()
{

    if (!enemy.isAlive())
    {
        currentState = GameState::GAME_OVER;
    }

    if (!player.isAlive())
    {
        currentState = GameState::GAME_OVER;
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
        // Render the enemy health
        renderAttackButtons();
        //Rebder Attack btns
        renderHealth(enemy.getHealth(), false);
        // Render the player's health
        renderHealth(player.getHealth(), true);
    }
    else if (currentState == GameState::GAME_OVER)
    {
        renderGameOver();
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

    // Adjust the Y-positions so attack buttons don't overlap
    for (int i = 0; i < 3; ++i)
    {
        SDL_Texture* textureToUse = nullptr;
        switch (attackButtonStates[i])
        {
        case NORMAL:
            textureToUse = buttonNormalTexture;
            break;
        case HOVER:
            textureToUse = buttonHoverTexture;
            break;
        case PRESSED:
            textureToUse = buttonPressedTexture;
            break;
        }

        // Ensure attack buttons are spaced well below the gladiator and not overlapping
        SDL_Rect rect = attackButtonRects[i];
        rect.y = 600; // Ensure buttons are placed lower
        TextureManager::render(textureToUse, renderer, rect);
    }
}


void Game::renderHealth(int health, bool isPlayer)
{
    // Convert health to string
    std::string healthText = (isPlayer ? "Player Health: " : "Goblin Health: ") + std::to_string(health);
    SDL_Color textColor = { 255, 0, 0 };  // Red colour for the text

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, healthText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // Position for the text
    SDL_Rect textRect;
    textRect.x = isPlayer ? (800 - textSurface->w - 10) : 10; // Player health on the right side, enemy on the left
    textRect.y = 10; // Y position
    textRect.w = textSurface->w; // Width of the text
    textRect.h = textSurface->h; // Height of the text

    //CAN CHANGE ALL OF THIS AND REPLACEWITH A IMAGE INSEAD OF SDL CREATING BOX????????????????

    // Create a rectangle larger than the text for the background box
    SDL_Rect backgroundRect = { textRect.x - 5, textRect.y - 5, textRect.w + 10, textRect.h + 10 };

    // Set colour to white and draw the background rectangle
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color
    SDL_RenderFillRect(renderer, &backgroundRect);

    // Render the text on top of the background
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    // Cleanup
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    // Reset the drawing color to black (or any color for further rendering)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}



void Game::renderGameOver()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set background colour
    SDL_RenderClear(renderer);

    // Display Game Over Message
    std::string gameOverText = "Game Over!";
    std::string restartText = "Press 'R' to Restart";

    SDL_Color textColor = { 255, 0, 0 };  // Red for the text

    // Render Game Over Text
    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, gameOverText.c_str(), textColor);
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_Rect gameOverRect = { (800 - gameOverSurface->w) / 2, 200, gameOverSurface->w, gameOverSurface->h };

    SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);

    // Render Restart Text
    SDL_Surface* restartSurface = TTF_RenderText_Solid(font, restartText.c_str(), textColor);
    SDL_Texture* restartTexture = SDL_CreateTextureFromSurface(renderer, restartSurface);
    SDL_Rect restartRect = { (800 - restartSurface->w) / 2, 300, restartSurface->w, restartSurface->h };

    SDL_RenderCopy(renderer, restartTexture, nullptr, &restartRect);

    // Cleanup
    SDL_FreeSurface(gameOverSurface);
    SDL_FreeSurface(restartSurface);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(restartTexture);
}


void Game::renderAttackButtons()
{
    for (int i = 0; i < 3; ++i)
    {
        SDL_Texture* buttonTexture = attackButtonTextures[i];
        SDL_Rect buttonRect = attackButtonRects[i];
        TextureManager::render(buttonTexture, renderer, buttonRect);
    }
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