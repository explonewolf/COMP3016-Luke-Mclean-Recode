# COMP3016 Luke Mclean RPG text-based game
https://github.com/explonewolf/COMP3016-Luke-Mclean-Recode

https://youtu.be/ZL9ky_bVAco


firstly delete save.txt
if using visual studio 2022 click project > retarget solution and click ok

#1. Dependencies Used
Standard Libraries:
<iostream>: For standard input and output.
<vector>: For managing dynamic arrays, particularly to store maps
<string>: For handling character data and text.
<fstream>: For file handling, such as saving/loading game states.
<cstdlib> and <ctime>: For random number generation
<algorithm>: For utilities such as sorting or finding elements in containers. (mainly map updating)

Windows-Specific Libraries:
<windows.h>: Used to manage console attributes like screen dimensions.
<conio.h>: For getch(), allows keyboard input without pressing enter



Ending Conditions:
The game has two endings based on player choices and actions, which are determined by the mercy count.
.
Different maps are loaded based on the player's progress and decisions, leading to various outcomes.



##the pdf in the submission contains UML diagrams and more in-depth on the code side 




















##########################################################################################################################################################################################################################################################################################


#what is in the pdf:






2. Programming patterns used


1. Component Pattern
The Character class holds attributes and behaviours related to a character/enemy in the game, such as health, attack range, and methods for attacking and showing mercy. This allows components to be added independently and easily. 

2. State Pattern
The game has different states (e.g., tutorial, various levels, shop) that dictate the behaviour. The map_filename variable determines the game's current map, and the logic changes based on this state. This pattern helps manage state transitions.

3. Command Pattern
The player's actions (e.g., moving, attacking, using items) are handled through commands. The input is processed in a loop, and different methods are called based on the action (like 'attack' or 'mercy'). This pattern separates the action from the input handling.


4. Factory Pattern
The creation of enemies based on the current map (e.g., different enemies and HP for different levels) can be seen as a factory pattern. The code randomly selects an enemy type based on the current level, using logic for enemy creation.










3. Game Mechanics and How They Are Coded

Combat System: The attack method in Character calculates damage based on an attack_range depending on weapons, adding randomness to combat interactions.

Health and Potions: players can use potions to regain health. Allows Players to overheal, but it gets reset when they go back to town.

Gold and Rewards: The gold attribute, can be obtained from defeating enemies, and can be spent in the shop in the town


Navigation or Choices: makes use of getch() to allow for easier movement for the user. As they will not have to press enter constantly. 


4. exception handling and test cases

 In the load_game function, there is a try-catch block that tries to read the game state from a file. If an error occurs during this process (e.g., if the file format is incorrect or the file is missing), it catches the exception and prints an appropriate error message. This was mainly implemented due to having issues when coding the save and load system.

If the user tries to put in an incorrect input appropriate messages are said to the user and the game does not crash. 

I have not written any formal test cases as I was constantly manually testing as I was going along and playing the game to make sure both endings were working correctly. 














Overview:
The game is a text-based RPG where the player navigates through different maps, interacts with characters, fights enemies, and makes choices that affect the game's outcome. The game uses a console user interface and is structured around a main loop that handles player input and game state updates.

Key Components

Character Class:
Represents both the player and enemies.
Contains attributes such as name, health, max_health, attack_range, weapon, health_potions, and gold.

Methods include:
attack(Character& other): Calculates and applies damage to another character.

mercy(): Displays a message indicating mercy was shown.

is_alive(): Checks if the character is still alive.

use_health_potion(): Heals the character if health potions are available.

Game State Management:

The game state is saved and loaded using the save_game and load_game functions, which read from and write to a file (save.txt).

The game state includes player and enemy health, position, map filename, weapon, health potions, and gold.

Map Handling:

Maps are loaded from text files using the load_map_from_file function, which reads/writes the map to a vector of strings.

The player’s position is updated based on input, and the game checks if the player can move to a new position using the can_move_to function.


Combat System:

The player can fight enemies using the fight_M function, which handles the fight logic.

The player can choose to attack or show mercy during combat.

The combat loop continues until either the player or the enemy is defeated.


User Interaction:
The game captures user input for movement and actions using _getch() stopping the need to press enter.

The player can interact with characters, open doors, and shop for items based on their position on the map.

Game Loop:
The main game loop continuously updates the game state, displays the current map and player status, and processes user input.

The loop checks for various conditions, such as encountering enemies, interacting with characters, or moving to different areas.

Visual Feedback:
The game uses ANSI escape codes to change text colours and backgrounds for visual feedback during combat and interactions.

The flash_screen function simulates a screen flashing red to indicate actions like attacks.

Ending Conditions:
The game has two endings based on player choices and actions, which are determined by the mercy count.
.
Different maps are loaded based on the player's progress and decisions, leading to various outcomes.


8. Evaluation and Reflection

Achievements: Successfully implemented a turn-based combat system, dynamic character attributes, and inventory management.

Challenges: Managing real-time inputs and handling Windows-specific dependencies like getch() and <windows.h>.

Future Improvements: Consider cross-platform support by removing Windows dependencies, adding more complex game states or AI for enemies, coming into the project more prepared, completely refactoring the code as it is currently hard to read, adding music using SDL  and implementing std::thread so I can run multiple threads to a time.  






















