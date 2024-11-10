<h1>Cyber Gladiator</h1>

<p>in this report i will document the creation of this game etc </p>


<h3>How to play Cyber Gladiator</h3>

- **Controls**:
  - Use the mouse to select an attack option from the 3 different attacks.
  - Each attack will do a random amount of damage.
  - The goblin will attack back after each attack.
  - To win, you must defeat the goblin before he defeats you


<h4>YouTube Link:</h4>

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

  

<h4>Game Mechanics and how they are coded</h4>
<p>ETC</p>

<h4>UML design diagrams</h4>
<p>ETC</p>

<h4>Exception handling and test cases</h4>
<p>ETC</p>

<h4>A brief evaluation of what i have achieved and what i would do differenlty</h4>
<p>ETC</p>
