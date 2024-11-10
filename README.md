<h1>Cyber Gladiator</h1>

<h3>How to play Cyber Gladiator</h3>

- **Controls**:
  - Use the mouse to select an attack option from the 3 different attacks.
  - Each attack will do a random amount of damage.
  - The goblin will attack back after each attack.
  - To win, you must defeat the goblin before he defeats you


<h4>YouTube Link: https://youtu.be/1z48uRbFQiE</h4>

<h2>How to Run the executable</h2>
<p>In order to run the executable, you must first unzip the file called “Game Files” and inside that folder will be a executable called “TurnBasedGame” which you can then double click to launch.</p>

<h2>Dependencies used</h2>
<p>In this project I have used multiple SDL dependencies such as SDL.h, which includes the main SDL2 library. This is used for handling graphics, events and other low-level operations.
SDL ttf, this is used for rendering text that work with TrueTypeFonts, and
SDL Mixer, this is used for handling audio in my game.</p>

<h2>Game Programming patterns used</h2>
<h4>State Pattern</h4>
<p>The currentState variable of GameState is used to manage the game's different states (MENU, PLAY, GAME_OVER, EXIT). The game updates and renders different components based on the current state. This is a characteristic of the State Pattern, where the behaviour of the system is dependent on its state.</p>
> - The game logic in handleEvents(), update(), and render() changes depending on whether the game is in the MENU, PLAY, or GAME_OVER state.
> - For example, in handleEvents(), it checks for the state before responding to button presses (if (currentState == GameState::MENU)).

  

<h2>Game Mechanics and how they are coded</h2>
<p>The game mechanics are based on a turn based combat system where the player and an enemy take turns attacking eachover.</p>
<h3>Game State mechanics:</h3>
<p>The game transitions between different states using the GameState enum.</p>
<p>Menu: Where the player can start the game.</p>
<p>Play: The actual combat phase where the player and enemy interact.</p>
<p>Game Over: When either the player or the enemy dies</p>


<h3>Player and Enemy Combat:</h3>
<p>The player and the enemy engage in combat where the player can choose different types of attacks. Each attack reduces health and the turn alternates between the player and the enemy.</p>
<p>Player Actions:</p>
<p>The player can select from a list of attacks (e.g., "Slash", "Kick", "Fireball").</p>
<p>•	After selecting an attack, the player's attack is executed, and the enemy takes damage.</p>
<p>•	If the enemy is still alive after the player's turn, it will counterattack</p>
<p>•	The game checks if either the player or the enemy has died (i.e., health reaches zero).</p>
<p>•The combat flow is controlled in the handleEvents() and update() functions:</p>
<p>•In handleEvents(), the player selects an attack via mouse click:</p>
<p>•The combat flow is controlled in the handleEvents() and update() functions:</p>
<p>player.selectAttack(i);        // Select attack based on button index</p>
<p>player.performAttack(enemy);   // Perform the selected attack</p>
<p>After the player's attack, the enemy can counterattack if it is still alive:</p>
<p>if (enemy.isAlive()) {
    player.receiveDamage(rand() % 30);}  // Enemy attacks the player</p>
<p>The game checks for victory conditions (game over) in the update() method:</p>
<p>if (!enemy.isAlive() || !player.isAlive()) {
    currentState = GameState::GAME_OVER;
}</p>

<h3>Health Management</h3>
<p>Both the player and the enemy have health points that are reduced when they take damage. The health is displayed in the UI, and when it reaches zero, the game ends.</p>
<p>The renderHealth() function renders the health of the player or the enemy:</p>
<p>renderHealth(enemy.getHealth(), false);</p>
<p>renderHealth(player.getHealth(), true);</p>
<p>The player's health is managed through the Player class and the enemy's health is managed through the Enemy class. Both classes have methods like receiveDamage(int) and isAlive() to manage health:</p>
<p>player.receiveDamage(rand() % 30);  // Random damage from the enemy</p>
<p>The health text is rendered as part of the game interface in the renderHealth() method, where the health is displayed in red colour:</p>
<p>SDL_Surface* textSurface = TTF_RenderText_Solid(font, healthText.c_str(), textColor);</p>

<h3>Attack Log:</h3>
<p>The game logs each attack, storing messages that describe the player's and enemy's actions. These logs are displayed in a box at the bottom of the screen.</p>
<p>Attack log messages are stored in a std::vector<std::string> attackLog and rendered in the renderAttackLog() method. The log is updated whenever a new attack occurs:</p>
<p>std::string attackMessage = player.getName() + " attacks " + enemy.getName() + " with " + player.getAttackOptions()[i] + ". ";</p>
<p>attackLog.push_back(attackMessage);</p>
<p>The renderAttackLog() method takes care of displaying the log with proper text wrapping:</p>
<p>std::vector<std::string> wrappedLines = wrapText(font, *it, attackLogRect.w - 2 * sidePadding);</p>
  

<h2>Exception handling and test cases</h2>
<p>I carried out various tests when creating this game to ensure there were no major problems at launch. Some of these tests included:</p>
<h3>Testing the game is properly initialised:</h3>
<p>Making sure the game window launched in the correct dimensions</p>
<p>Making sure the SDL libraries and components like SDL_ttf and SDL_mixer are properly initialised</p>
<p>Ensuring the background music plays at launch, and the start menu has the correct background texture</p>
<p>Making sure the start button functions</p>

<h3>Testing the Game States and Transitions:</h3>
<p>Verifying transition from MENU to PLAY state</p>
<p>Verify transition from PLAY to GAME_OVER state</p>
<p>Verify transition from GAME_OVER to MENU state</p>

<h3>Testing the Combat System:</h3>
<p>Ensure the player can perform an attack</p>
<p>Ensure the Enemy can perform an attack, and make sure it happens after the player attacks</p>
<p>Verify correct calculation of damage</p>
<p>Test player's health depletion</p>
<p>Test enemy’s health depletion</p>

<h3>Testing the UI and Interaction:</h3>
<p>Verify button states (Normal, Hover, Pressed)</p>
<p>Verify correct attack buttons are displayed and interactable</p>
<p>Test attack log displays correctly</p>
<p>Verify health UI renders correctly</p>


<h2>A brief evaluation of what i have achieved and what i would do differenlty</h2>
<p>I am very confident and happy in what I have achieved just by using SDL and C++, this was my first time using and experimenting with SDL and I feel like I have learnt a lot during this project. I also am happy with the progress I have made in strengthening my c++ skills as I now feel more confident in working with classes and object-oriented based code. What i would do differently if I had to do a project similar to this again is learn how to use animation in SDL, I’m not sure if there is a method on making animation using SDL but if there is i would want to learn about it and implement it into my work. I also would like to have added more enemies and a lives system so the player could keep playing after defeating an enemy, but unfortunately, I did not have enough time.</p>
<p>Thank you for taking the time to read this report - Morgan Hodge</p>
