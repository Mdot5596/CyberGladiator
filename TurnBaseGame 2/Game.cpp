#include "Game.h"
#include "TextureManager.h"
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <sstream>
#include <vector>

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), currentState(GameState::MENU), player("Cyber Gladiator"), enemy("Goblin") {} //to long lol


Game::~Game()
{
    clean();
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    width = 1024;  // Override the width to 1024
    height = 768;  // Override the height to 768


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
        font2 = TTF_OpenFont("assets/Caviar_Dreams_Bold.ttf", 12); //CHANGE - FONT SUCKS ATM

        // Load Textures
        menuBackgroundTexture = TextureManager::loadTexture("assets/background1.png", renderer);//SHIT
        buttonNormalTexture = TextureManager::loadTexture("assets/STARTBTN.png", renderer);//GOOD
        buttonHoverTexture = TextureManager::loadTexture("assets/startbown.png", renderer);//GOOD
        backgroundTexture = TextureManager::loadTexture("assets/TESTWALL1.png", renderer);//SHIT
        gladiatorTexture = TextureManager::loadTexture("assets/gladiator.png", renderer);//SHIT
        goblinTexture = TextureManager::loadTexture("assets/goblin.png", renderer);//good
        attackButtonTextures[0] = TextureManager::loadTexture("assets/slashbtn.png", renderer);
        attackButtonTextures[1] = TextureManager::loadTexture("assets/kickbtn.png", renderer);
        attackButtonTextures[2] = TextureManager::loadTexture("assets/fireballbtn.png", renderer);
        attackLogTexture = TextureManager::loadTexture("assets/attacklog1.png", renderer);

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

            for (int i = 0; i < 3; ++i) {
                if (attackButtonStates[i] == HOVER && currentState == GameState::PLAY) {
                    player.selectAttack(i);        // Select attack based on button index
                    player.performAttack(enemy);   // Perform the selected attack

                    // Add attack log message
                    std::string attackMessage = player.getName() + " attacks " + enemy.getName() +
                        " with " + player.getAttackOptions()[i] + ". ";
                    attackMessage += enemy.getName() + "'s health: " + std::to_string(enemy.getHealth());
                    attackLog.push_back(attackMessage);  // Store the attack message

                    SDL_Delay(1000);  // Delay to simulate time between attacks

                    // Enemy attacks if still alive
                    if (enemy.isAlive())
                    {
                        std::cout << enemy.getName() << " attacks " << player.getName() << "!\n";
                        player.receiveDamage(15);  // Example damage

                        // Log enemy attack
                        std::string enemyAttackMessage = enemy.getName() + " attacks " + player.getName() +
                            ". " + player.getName() + "'s health: " + std::to_string(player.getHealth());
                        attackLog.push_back(enemyAttackMessage);  // Store enemy attack message
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
                //clears attacklog after each game
                attackLog.clear();
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
        renderAttackLog();
        renderAttackButtons();
        renderHealth(enemy.getHealth(), false);
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

    SDL_Rect gladiatorRect = { 700, 184, 400, 400 }; // Gladiator position and size
    TextureManager::render(gladiatorTexture, renderer, gladiatorRect);


    SDL_Rect goblinRect = { -50, 184, 400, 400 };
    TextureManager::render(goblinTexture, renderer, goblinRect);

  


    renderAttackLog();

   //FIXED THE MLTPL BUTTON ISSUE BY REMOVING THE SWITCH STATMENT HERER
}



void Game::renderHealth(int health, bool isPlayer)
{
    // Convert health to string
    std::string healthText = (isPlayer ? "Player Health: " : "Goblin Health: ") + std::to_string(health);
    SDL_Color textColor = { 255, 0, 0 };  // Red colour for the text

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, healthText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect;
    if (isPlayer) {
        textRect.x = 700; // Adjust for player's health text position (further to the left)
        textRect.y = 50;  // Lower the player's health text
    }
    else {
        textRect.x = 100; // Adjust for goblin's health text position (near left side)
        textRect.y = 50;  // Higher up for goblin's health text
    }
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

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



// Utility function to split text into lines that fit within a specified width
std::vector<std::string> wrapText(TTF_Font* font, const std::string& text, int maxWidth)
{
    std::vector<std::string> wrappedLines;
    std::istringstream words(text);
    std::string word, line;

    while (words >> word) 
    {
        std::string testLine = line + (line.empty() ? "" : " ") + word;

        int textWidth;
        TTF_SizeText(font, testLine.c_str(), &textWidth, nullptr);

        if (textWidth > maxWidth) 
        {
            if (!line.empty()) wrappedLines.push_back(line); // Push current line if it has content
            line = word;
        }
        else 
        {
            line = testLine;
        }
    }

    if (!line.empty()) wrappedLines.push_back(line); // Add the final line if it's not empty

    return wrappedLines;
}


void Game::renderAttackLog()
{
    // Render the background of the attack log
    TextureManager::render(attackLogTexture, renderer, attackLogRect);

    // Define padding and text parameters
    const int sidePadding = 80;
    const int verticalPadding = 100;
    const int lineHeight = TTF_FontHeight(font);
    const int maxVisibleLines = (attackLogRect.h - 2 * verticalPadding) / lineHeight;

    int yOffset = attackLogRect.y + verticalPadding;
    int visibleLinesCount = 0;

    for (auto it = attackLog.rbegin(); it != attackLog.rend() && visibleLinesCount < maxVisibleLines; ++it) 
    {
        std::vector<std::string> wrappedLines = wrapText(font, *it, attackLogRect.w - 2 * sidePadding);

        for (const auto& line : wrappedLines)
        {
            if (visibleLinesCount >= maxVisibleLines) break;

            SDL_Surface* textSurface = TTF_RenderText_Solid(font, line.c_str(), { 255, 255, 255 });
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            SDL_Rect textRect = { attackLogRect.x + sidePadding, yOffset, textSurface->w, textSurface->h };
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

            yOffset += lineHeight;
            visibleLinesCount++;

            //Clean
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}

void Game::clean()
{
    // Clean up textures and SDL
    TextureManager::clean(backgroundTexture);
    TextureManager::clean(gladiatorTexture);
    TextureManager::clean(goblinTexture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(attackLogTexture);
    TTF_CloseFont(font);  // Close the font
    TTF_Quit();  // Quit TTF
    SDL_Quit();
}

bool Game::running() const
{
    return isRunning;
}