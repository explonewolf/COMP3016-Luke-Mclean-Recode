// 3016 take 2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <conio.h> // For getch() on Windows
#include <windows.h> // For GetConsoleScreenBufferInfo
#include "functions.h"
#include <algorithm>
#include <windows.h>

int get_terminal_width() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    } else {
        columns = 80; // Default width if unable to get terminal size
    }
    return columns;
}

int get_terminal_height() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int rows;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    } else {
        rows = 24; // Default height if unable to get terminal size
    }
    return rows;
}

void display_legend(int terminal_width) {
    std::string legend[] = {
        "Legend:",
        "Monsters: M",
        "People: P",
        "Character: @",
        "Trees: T",
        "Doors: D"
    };

    for (const auto& line : legend) {
        int padding = terminal_width - line.length();
        std::cout << std::string(padding, ' ') << line << std::endl;
    }
}

void display_instructions() {
    std::cout << "Instructions:\n"
              << "Use 'w', 'a', 's', 'd' to move.\n"
              << "Press 'e' to interact.\n"
              << "Defeat enemies to win.\n"
              << std::endl;
}

void display_map(const std::vector<std::string>& map, int player_x, int player_y, int terminal_width, int terminal_height) {
    int vertical_padding = (terminal_height - map.size()) / 2;
    for (int i = 0; i < vertical_padding; ++i) {
        std::cout << std::endl;
    }
    for (int y = 0; y < map.size(); ++y) {
        std::string line = map[y];
        if (y == player_y) {
            line[player_x] = '@'; // Player position
        }
        int horizontal_padding = (terminal_width - line.size()) / 2;
        std::cout << std::string(horizontal_padding, ' ') << line << std::endl;
    }
}

bool is_next_to_p(int player_x, int player_y, const std::vector<std::string>& map) {
    return (player_x > 0 && map[player_y][player_x - 1] == 'P') ||
           (player_x < map[0].size() - 1 && map[player_y][player_x + 1] == 'P') ||
           (player_y > 0 && map[player_y - 1][player_x] == 'P') ||
           (player_y < map.size() - 1 && map[player_y + 1][player_x] == 'P');
}

bool is_next_to_s(int player_x, int player_y, const std::vector<std::string>& map) {
    return (player_x > 0 && map[player_y][player_x - 1] == 'S') ||
           (player_x < map[0].size() - 1 && map[player_y][player_x + 1] == 'S') ||
           (player_y > 0 && map[player_y - 1][player_x] == 'S') ||
           (player_y < map.size() - 1 && map[player_y + 1][player_x] == 'S');
}

bool is_next_to_m(int player_x, int player_y, const std::vector<std::string>& map) {
    return (player_x > 0 && map[player_y][player_x - 1] == 'M') ||
           (player_x < map[0].size() - 1 && map[player_y][player_x + 1] == 'M') ||
           (player_y > 0 && map[player_y - 1][player_x] == 'M') ||
           (player_y < map.size() - 1 && map[player_y + 1][player_x] == 'M');
}

void ending_screen(const std::vector<std::string>& map,int terminal_width, int terminal_height) {
    system("cls");
     int vertical_padding = (terminal_height - map.size()) / 2;
    for (int i = 0; i < vertical_padding; ++i) {
        std::cout << std::endl;
    }
    for (int y = 0; y < map.size(); ++y) {
        std::string line = map[y];
        int horizontal_padding = (terminal_width - line.size()) / 2;
        std::cout << std::string(horizontal_padding, ' ') << line << std::endl;
    }
}

std::vector<std::string> load_map_from_file(const std::string& filename) {
    std::vector<std::string> map;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        map.push_back(line);
    }
    return map;
}

bool can_move_to(int x, int y, const std::vector<std::string>& map) {
    return map[y][x] != 'T' && map[y][x] != 'P' && map[y][x] != 'D' && map[y][x] != 'M' && map[y][x] != 'S' && map[y][x] != 'B'; // Check if the target position is not a tree, person, door, monster, Switch or barrier
}

void display_header(int terminal_width, Character& player) {
    std::string instructions[] = {
        "Instructions:",
        "Use 'w', 'a', 's', 'd' to move.",
        "Press 'e' to interact.",
        "Press 'h' to use a health potion.",
        "health potions: " + std::to_string(player.health_potions),
        "gold: " + std::to_string(player.gold),
        "hp: " + std::to_string(player.health),
        "Defeat enemies to win."
    };

    std::string legend[] = {
        "Legend:",
        "Monsters: M",
        "People: P",
        "Character: @",
        "Trees: T",
        "Doors: D"
    };

    int max_lines = std::max(sizeof(instructions) / sizeof(instructions[0]), sizeof(legend) / sizeof(legend[0]));

    for (int i = 0; i < max_lines; ++i) {
        if (i < sizeof(instructions) / sizeof(instructions[0])) {
            std::cout << instructions[i];
        }

        int padding = terminal_width - (i < sizeof(legend) / sizeof(legend[0]) ? legend[i].length() : 0);
        if (padding > instructions[i].length()) {
            padding -= instructions[i].length();
        } else {
            padding = 0;
        }

        if (i < sizeof(legend) / sizeof(legend[0])) {
            std::cout << std::string(padding, ' ') << legend[i];
        }

        std::cout << std::endl;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation

    std::string map_filename = "maps/Tutorial.txt"; // Default map
    std::vector<std::string> map = load_map_from_file(map_filename);

    int player_x = 1, player_y = 1; // Initial player position
    int mercy_count = 0; // Initialize mercy_count

    // Declare player and enemy outside the loop
    Character player("@", 100);
    Character enemy("Tutorial Enemy", 50);

    // Load game state
    load_game(player, enemy, mercy_count, player_x, player_y, map_filename);
    map = load_map_from_file(map_filename);

    while (true) {
        system("cls"); // Clear the console (Windows-specific)
        int terminal_width = get_terminal_width();
        int terminal_height = get_terminal_height();

        display_header(terminal_width, player);
        display_map(map, player_x, player_y, terminal_width, terminal_height);

        std::cout << "Move (w/a/s/d) or interact (e): ";
        char action = _getch(); // Use _getch() to capture key press without enter

        int new_x = player_x;
        int new_y = player_y;

        if (action == 'h') {
            Beep(650, 100);
            player.use_health_potion();
        } else if (action == 'w' && player_y > 0) {
            Beep(50, 100);
            new_y--;
        } else if (action == 's' && player_y < map.size() - 1) {
            Beep(50, 100);
            new_y++;
        } else if (action == 'a' && player_x > 0) {
            Beep(50, 100);
            new_x--;
        } else if (action == 'd' && player_x < map[0].size() - 1) {
            Beep(50, 100);
            new_x++;
        }

        // Check if the new position is valid (not a tree, person, or door)
        if (can_move_to(new_x, new_y, map)) {
            player_x = new_x;
            player_y = new_y;
        }

        if (action == 'e' && is_next_to_p(player_x, player_y, map) && map_filename == "maps/Tutorial.txt") {
            std::cout << "Starting tutorial fight!\n";
            
            // Start tutorial fight
            int enemy_max_health = enemy.health;

            while (player.is_alive() && enemy.is_alive()) {
                display_health(player, enemy);
                std::string action;
                
                // New line: Tutorial monster prompts the player to attack
                std::cout << "\033[33mTutorial Monster: Type 'attack' to attack me.\033[0m\n";
                
                if (enemy.health < 0.2 * enemy_max_health) {
                    std::cout << "\033[33mMercy is available!\n\033[0m";
                    
                    // New line: Tutorial monster suggests showing mercy
                    std::cout << "\033[33mTutorial Monster: In this world, it is always polite to show mercy to your opposition, especially if you want to be liked.\033[0m\n";
                }
                
                std::cout << "Choose action: (attack/mercy): ";
                std::cin >> action;

                if (action == "attack") {
                    player_attack(player, enemy); // Player attacks
                    enemy_attack(enemy, player); // Enemy attacks back
                } else if (action == "mercy") {
                    if (enemy.health < 0.2 * enemy_max_health) { // Check if enemy's health is below 20%
                        std::cout << "You showed mercy and ended the fight!\n";
                        std::cout << "\033[33mTutorial Monster: Thank you for showing mercy. I will never forget this. You are ready\033[0m\n"; 
                        Sleep(5000);
                        mercy_count++; // Increment mercy_count
                        break; // End the fight
                    } else {
                        player.mercy();
                    }
                } else {
                    std::cout << "Invalid action!\n";
                }
            }

            if (player.is_alive()) {
                if (mercy_count == 0) { std::cout << "\033[31mTutorial Monster: You monster you killed me\033[0m\n"; }
                
                std::cout << "\033[33mYou defeated the enemy and received a Wooden Sword!\033[0m\n";
                player.attack_range = {15, 20};
                player.health_potions += 5; 
                player.gold += 10;
                std::cout << "You received 5 health potions and 10 gold!\n";
                Sleep(3000);

                for (auto& row : map) {
                    std::replace(row.begin(), row.end(), 'P', ' ');
                }
            } else {
                std::cout << "You have been defeated!\n";
                break; // Exit the game loop if the player is defeated
            }
        } else if (action == 'e' && is_next_to_door(player_x, player_y, map)&& map_filename == "maps/Tutorial.txt") {
            std::cout << "You opened the door and entered the next area!\n";
            map_filename = "maps/level1.txt"; // Update map filename
            map = load_map_from_file(map_filename); // Load the next map
            player_x = 1; // Reset player position or set to desired spawn point
            player_y = 1; // Reset player position or set to desired spawn point
            save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state

        } else if (action == 'e' && is_next_to_m(player_x, player_y, map)) {
            //std::cout << "You encountered a monster!\n";
            //fight_M(player, map_filename); //true == player won, false == player lost
            if (fight_M(player, map_filename, mercy_count)) {
                Sleep(3000);
                if (player_x > 0 && map[player_y][player_x - 1] == 'M') {
                    map[player_y][player_x - 1] = ' ';
                } else if (player_x < map[0].size() - 1 && map[player_y][player_x + 1] == 'M') {
                    map[player_y][player_x + 1] = ' ';
                } else if (player_y > 0 && map[player_y - 1][player_x] == 'M') {
                    map[player_y - 1][player_x] = ' ';
                } else if (player_y < map.size() - 1 && map[player_y + 1][player_x] == 'M') {
                    map[player_y + 1][player_x] = ' ';
                }
            }
            else {
                std::cout << "You have been defeated!\n";
                break; // Exit the game loop if the player is defeated
            }
            
        } else if (action == 'e' && is_next_to_p(player_x, player_y, map) && map_filename == "maps/level1.txt") {
            system("cls");
            //std::cout << "You encountered a person!\n";
            int remaining_enemies = 0;
            for (const auto& row : map) {
                remaining_enemies += std::count(row.begin(), row.end(), 'M');
            }
            if (remaining_enemies > 0) {
                std::cout << "Please come back when you defeat the " << remaining_enemies << " remaining enemy(s)!\n";
                Sleep(3000);
                
            } else {
                if (mercy_count > 2) {
                    std::cout << "thank you for chasing the creatures away \n";
                    std::cout << "let me bring you to the village\n";
                }
                else {
                    std::cout << "\033[31mYOU MONSTER YOU KILLED THEM ALL\033[0m\n";
                    Sleep(1000);
                    system("cls");
                    std::cout << "let me bring you to the village\n";
                    Sleep(2000);
                    system("cls");
                    std::cout << "\033[31mI WILL NEVER FORGIVE YOU\033[0m\n"; 
                    Sleep(1000);
                    system("cls");
                    std::cout << "what are you waiting for? come with me\n";


                }
                map_filename = "maps/town.txt"; // Update map filename
                map = load_map_from_file(map_filename); // Load the next map
                player_x = 8; // Reset player position or set to desired spawn point
                player_y = 2; // Reset player position or set to desired spawn point
                player.health = player.max_health;
                save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state

                std::cout << "\033[32mwelcome to the town\n";
                Sleep(2000);
                std::cout << "you can buy weapons and armor here and restock up on health potions\n";
                Sleep(2000);
                std::cout << "you can also sell your weapons and armor here\n";
                Sleep(2000);
                std::cout << "your hp will refill here as well\n";
                Sleep(2000);
                std::cout << "come back when you are ready to continue your journey\n\033[0m";
                Sleep(2000);
            }
            
        } else if (action == 'e' && is_next_to_door(player_x, player_y, map) && map_filename == "maps/town.txt") {
            std::cout << "\033[32mYou opened the door and entered the shop\033[0m\n";
            Sleep(2000);
            map_filename = "maps/shop.txt"; // Update map filename
            map = load_map_from_file(map_filename); // Load the next map
            player_x = 9; // Reset player position or set to desired spawn point
            player_y = 2; // Reset player position or set to desired spawn point
            save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
        } else if (action == 'e' && is_next_to_door(player_x, player_y, map) && map_filename == "maps/shop.txt") {
            std::cout << "\033[32mYou opened the door and entered the town\033[0m\n";
            Sleep(2000);
            map_filename = "maps/town.txt"; // Update map filename
            map = load_map_from_file(map_filename); // Load the next map
            player_x = 5; // Reset player position or set to desired spawn point
            player_y = 2; // Reset player position or set to desired spawn point
            save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
        } else if (action == 'e' && is_next_to_p(player_x, player_y, map) && map_filename == "maps/shop.txt") {
            system("cls");
            std::cout << "hello adventurer, how can i help you today?\n";
            Sleep(2000);
            std::cout << "I have a wide variety of weapons and potions for sale\n";
            Sleep(2000);
            std::cout << "what would you like to do?\n";
            Sleep(2000);
            std::cout << "1. buy\n";
            std::cout << "2. sell\n";
            std::cout << "3. leave\n";
            std::cout << "please enter the number of the action you would like to take\n";
            int choice;
            std::cin >> choice;
            if (choice == 1) {
                std::cout << "what would you like to buy?\n";
                std::cout << "1. Stone Sword\n"; 
                std::cout << "2. health potion\n";
                std::cout << "3. leave\n";
                std::cin >> choice;
                if (choice == 1) {
                    if (player.gold >= 10) {
                        player.gold -= 10;
                        player.attack_range = {20, 30};
                        std::cout << "You bought a Stone Sword!\n";
                        Sleep(2000);
                    }
                    else {
                        std::cout << "Come back when you have money\n";
                        Sleep(2000);
                    }
                } else if (choice == 2) {
                    if (player.gold >= 10) {
                        player.gold -= 10;
                        player.health_potions += 5;
                        std::cout << "You bought a health potion!\n";
                        Sleep(2000);
                    }
                    else {
                        std::cout << "Come back when you have money!!!\n";
                        Sleep(2000);
                    }
                }
                else if (choice == 3) {
                    std::cout << "NO WINDOW SHOPPING!!!\n";
                    Sleep(2000);
                }
            } else if (choice == 2) {
                std::cout << "come back when you have something to sell!!!\n";
                Sleep(2000);
            } else if (choice == 3) {
                std::cout << "NO WINDOW SHOPPING!!!\n";
                Sleep(2000);
            } else {
                std::cout << "invalid choice\n";
                Sleep(2000);
            }
        } else if (action == 'e' && is_next_to_p(player_x, player_y, map) && map_filename == "maps/town.txt") {
            std::cout << "\nare you ready to continue your journey?\n";
            Sleep(2000);
            std::cout << "1. yes\n";
            std::cout << "2. no\n";
            int choice;
            std::cin >> choice;
            if (choice == 1) {
                map_filename = "maps/level2.txt"; // Update map filename
                map = load_map_from_file(map_filename); // Load the next map
                player_x = 1; // Reset player position or set to desired spawn point
                player_y = 1; // Reset player position or set to desired spawn point
                save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
            }
            else {
                std::cout << "come back when you are ready to continue your journey\n";
                Sleep(2000);
            }
        } else if (action == 'e' && is_next_to_p(player_x, player_y, map) && map_filename == "maps/level2.txt") {
            int remaining_enemies = count_enemies(map);
            if (remaining_enemies > 0) {
                std::cout << "Please come back when you defeat the " << remaining_enemies << " remaining enemy(s)!\n";
                Sleep(2000);
            }
            else {
                if (mercy_count < 1) {
                    std::cout << "\n\033[31mYOU MONSTER YOU KILLED THEM ALL\033[0m\n";
                    Sleep(1000);
                    system("cls");
                    std::cout << "let me bring you to the village\n";
                    Sleep(1000);
                    system("cls");
                    std::cout <<"\033[31mI WILL NEVER FORGIVE YOU\033[0m\n";
                    Sleep(1000);
                    system("cls");
                    std::cout << "what are you waiting for? come with me\n";
                    Sleep(2000);
                }
                else {
                    std::cout << "thank you for chasing the creatures away\n";
                    Sleep(2000);
                    std::cout << "lets go back to the town\n";
                    Sleep(2000);
                    
                }
                player.health = player.max_health;
                map_filename = "maps/town2.txt"; // Update map filename
                map = load_map_from_file(map_filename); // Load the next map
                player_x = 8; // Reset player position or set to desired spawn point
                player_y = 2; // Reset player position or set to desired spawn point
                save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
            }
        }else if (action == 'e' && is_next_to_door(player_x, player_y, map) && map_filename == "maps/town2.txt") {
            std::cout << "\033[32mYou opened the door and entered the shop\033[0m\n";
            Sleep(2000);
            map_filename = "maps/shop2.txt"; // Update map filename
            map = load_map_from_file(map_filename); // Load the next map
            player_x = 9; // Reset player position or set to desired spawn point
            player_y = 2; // Reset player position or set to desired spawn point
            save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
        } else if (action == 'e' && is_next_to_p(player_x, player_y, map) && map_filename == "maps/shop2.txt") {
            std::cout << "hello adventurer, how can i help you today?\n";
            std::cout << "I have a wide variety of weapons and potions for sale\n";
            std::cout << "what would you like to do?\n";
            std::cout << "1. buy\n";
            std::cout << "2. sell\n";
            std::cout << "3. leave\n";
            std::cout << "please enter the number of the action you would like to take\n";
            int choice;
            std::cin >> choice;
            if (choice == 1) {
                std::cout << "what would you like to buy?\n";
                std::cout << "1. Steel Sword\n";
                std::cout << "2. health potion\n";
                std::cout << "3. leave\n";
                std::cin >> choice;
                if (choice == 1) {
                    if (player.gold >= 20) {
                        player.gold -= 20;
                        player.attack_range = {30, 40};
                        std::cout << "You bought a Steel Sword!\n";
                        Sleep(2000);
                    }
                    else {
                        std::cout << "come back when you have money\n";
                        Sleep(2000);
                    }
                }
                else if (choice == 2) {
                    if (player.gold >= 10) {
                        player.gold -= 10;
                        player.health_potions += 5;
                        std::cout << "You bought a health potion!\n";
                        Sleep(2000);
                    }
                    else {
                        std::cout << "come back when you have money\n";
                        Sleep(2000);
                    }
                }
                else if (choice == 3) {
                    std::cout << "NO WINDOW SHOPPING!!!\n";
                    Sleep(2000);
                }
            }
            if (choice == 2) {
                std::cout << "come back when you have something to sell!!!\n";
                Sleep(2000);
            }
            if (choice == 3) {
                std::cout << "NO WINDOW SHOPPING!!!\n";
                Sleep(2000);
            }
        }else if (action == 'e' && is_next_to_door(player_x, player_y, map) && map_filename == "maps/shop2.txt") {
            std::cout << "\033[32mYou opened the door and entered the town\033[0m\n";
            Sleep(2000);
            map_filename = "maps/town2.txt"; // Update map filename
            map = load_map_from_file(map_filename); // Load the next map
            player_x = 5; // Reset player position or set to desired spawn point
            player_y = 2; // Reset player position or set to desired spawn point
            save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
        }else if (action == 'e' && is_next_to_p(player_x, player_y, map) && map_filename == "maps/town2.txt") {
            std::cout << "are you ready to continue your journey?\n";
            Sleep(2000);
            std::cout << "1. yes\n";
            std::cout << "2. no\n";
            int choice;
            std::cin >> choice;
            if (choice == 1) {
                map_filename = "maps/level3.txt"; // Update map filename
                map = load_map_from_file(map_filename); // Load the next map
                player_x = 1; // Reset player position or set to desired spawn point
                player_y = 1; // Reset player position or set to desired spawn point
                save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
            }
            else {
                std::cout << "come back when you are ready to continue your journey\n";
                Sleep(2000);
            }
        }else if (action == 'e' && is_next_to_s(player_x, player_y, map) && map_filename == "maps/level3.txt") {
            std::cout << "You encountered a Switch\n";
            Sleep(2000);
            if (count_enemies(map) > 0) {
                std::cout << "\nI better get rid of these pesky creatures first\n";
                Sleep(2000);
            }
            else {
                std::cout << "\nI can now activate the switch\n";
                Beep(350, 300); // Make a beep sound
                for (auto& row : map) {
                    std::replace(row.begin(), row.end(), 'S', ' ');
                }
                for (auto& row : map) {
                    std::replace(row.begin(), row.end(), 'B', ' ');
                }
                Sleep(2000);
                std::cout <<" the switch has broken and the door has opened\n";
                Sleep(2000);
            }   
        }else if (action == 'e' && is_next_to_p(player_x, player_y, map) && map_filename == "maps/level3.txt") {
            if (mercy_count > 0) {
                std::cout << "thank you for chasing the creatures away and freeing me\n";
                Sleep(2000);
                std::cout << "lets go back to the town\n";
                Sleep(2000);
            }
            else {
                std::cout << "\033[31mYOU ARE A MONSTER YOU KILLED THEM ALL\033[0m\n";
                Sleep(1000);
                system("cls");
                std::cout << "\033[31mYOU WILL PAY FOR NOT FOLLOWING THE RULES MONSTER\033[0m\n";
                Sleep(4000);
                system("cls");
                std::cout << "\033[31mMONSTER\033[0m\n";
                Sleep(7000);
                system("cls");
                std::cout << "lets go back to the town\n";
                Sleep(2000);
            }
            player.health = player.max_health;
            map_filename = "maps/town3.txt"; // Update map filename
            map = load_map_from_file(map_filename); // Load the next map
            player_x = 8; // Reset player position or set to desired spawn point
            player_y = 2; // Reset player position or set to desired spawn point
            save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
        }else if (action == 'e' && is_next_to_door(player_x, player_y, map) && map_filename == "maps/town3.txt") {
            std::cout << "\033[32mYou opened the door and entered the shop\033[0m\n";
            Sleep(2000);
            map_filename = "maps/shop3.txt"; // Update map filename
            map = load_map_from_file(map_filename); // Load the next map
            player_x = 9; // Reset player position or set to desired spawn point
            player_y = 2; // Reset player position or set to desired spawn point
            save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
        }else if (action == 'e' && is_next_to_p(player_x, player_y, map) && map_filename == "maps/shop3.txt") {
            std::cout << "hello adventurer, how can i help you today?\n";
            std::cout << "I have a wide variety of weapons and potions for sale\n"; 
            std::cout << "what would you like to do?\n";
            std::cout << "1. buy\n";
            std::cout << "2. sell\n";
            std::cout << "3. leave\n";
            std::cout << "please enter the number of the action you would like to take\n";
            int choice;
            std::cin >> choice;
            if (choice == 1) {
                std::cout << "what would you like to buy?\n";
                std::cout << "1. Iron Sword\n";
                std::cout << "2. health potion\n";
                std::cout << "3. leave\n";
                std::cin >> choice;
                if (choice == 1) {
                    if (player.gold >= 30) {
                        player.gold -= 30;
                        player.attack_range = {40, 50};
                        std::cout << "You bought an Iron Sword!\n";
                        Sleep(2000);
                    }
                    else {
                        std::cout << "come back when you have money\n";
                        Sleep(2000);
                    }
                }
                else if (choice == 2) {
                    if (player.gold >= 10) {
                        player.gold -= 10;
                        player.health_potions += 5;
                    }
                    else {
                        std::cout << "come back when you have money\n";
                        Sleep(2000);
                    }
                }
                else if (choice == 3) {
                    std::cout << "NO WINDOW SHOPPING!!!\n";
                    Sleep(2000);
                }
                
            }
            else if (choice == 2) {
                std::cout << "come back when you have something to sell!!!\n";
                Sleep(2000);
            }
            else if (choice == 3) {
                std::cout << "NO WINDOW SHOPPING!!!\n";
                Sleep(2000);
            }
        }else if (action == 'e' && is_next_to_door(player_x, player_y, map) && map_filename == "maps/shop3.txt") {
            std::cout << "\033[32mYou opened the door and entered the town\033[0m\n";
            Sleep(2000);
            map_filename = "maps/town3.txt"; // Update map filename
            map = load_map_from_file(map_filename); // Load the next map
            player_x = 5; // Reset player position or set to desired spawn point
            player_y = 2; // Reset player position or set to desired spawn point
            save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
        }else if (action == 'e' && is_next_to_p(player_x, player_y, map) && map_filename == "maps/town3.txt") {
            std::cout << "are you ready to continue your journey?\n";
            Sleep(2000);
            std::cout << "1. yes\n";
            std::cout << "2. no\n";
            int choice;
            std::cin >> choice;
            if (choice == 1) {
                map_filename = "maps/level4.txt"; // Update map filename
                map = load_map_from_file(map_filename); // Load the next map
                player_x = 1; // Reset player position or set to desired spawn point
                player_y = 1; // Reset player position or set to desired spawn point
                save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
            }
            else {
                std::cout << "come back when you are ready to continue your journey\n";
                Sleep(2000);
            }
        }else if (action == 'e' && is_next_to_s(player_x, player_y, map) && map_filename == "maps/level4.txt") {
            std::cout << "You encountered a Switch\n";
            Sleep(2000);
            if (count_enemies(map) > 0) {
                std::cout << "\nI better get rid of these pesky creatures first\n";
                Sleep(2000);
            }
            else {
                std::cout << "\nI can now activate the switch\n";
                Beep(350, 300); // Make a beep sound
                Sleep(2000);
                for (auto& row : map) {
                    std::replace(row.begin(), row.end(), 'S', ' ');
                }
                for (auto& row : map) {
                    std::replace(row.begin(), row.end(), 'B', ' ');
                }
                Sleep(2000);
                std::cout << "the switch has broken and the door has opened\n";
                Sleep(2000);
            }
        }else if (action == 'e' && is_next_to_p(player_x, player_y, map) && map_filename == "maps/level4.txt") {
            if (mercy_count > 0) {
                std::cout << "thank you for chasing the creatures away and freeing me\n";
                Sleep(2000);
                std::cout << "lets go back to the town\n";
                Sleep(2000);
            }
            else {
                std::cout << "\033[31mYOU WILL PAY FOR YOUR CRUELTY\033[0m\n";
                Sleep(1000);
                system("cls");
                for (int i = 0; i < 100; i++) {
                    std::cout << "\033[31mDIE\033[0m\n";
                    Sleep(100);
                }
                std::cout << "\n\033[31mYOU MONSTER\033[0m\n";
                Sleep(1000);
                system("cls");
                std::cout << "lets go back to the town\n";
                Sleep(2000);
            }
            player.health = player.max_health;
            map_filename = "maps/town4.txt"; // Update map filename
            map = load_map_from_file(map_filename); // Load the next map
            player_x = 8; // Reset player position or set to desired spawn point
            player_y = 2; // Reset player position or set to desired spawn point
            save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
        } else if (action == 'e' && is_next_to_door(player_x, player_y, map) && map_filename == "maps/town4.txt") {
            std::cout << "\033[32mYou opened the door and entered the shop\033[0m\n";
            Sleep(2000);
            map_filename = "maps/shop4.txt"; // Update map filename
            map = load_map_from_file(map_filename); // Load the next map
            player_x = 9; // Reset player position or set to desired spawn point
            player_y = 2; // Reset player position or set to desired spawn point
            save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
        }else if (action == 'e' && is_next_to_p(player_x, player_y, map) && map_filename == "maps/shop4.txt") {
            std::cout << "hello adventurer, how can i help you today?\n";
            std::cout << "I have a wide variety of weapons and potions for sale\n";
            std::cout << "what would you like to do?\n";
            std::cout << "1. buy\n";
            std::cout << "2. sell\n";
            std::cout << "3. leave\n";
            std::cout << "please enter the number of the action you would like to take\n";
            int choice;
            std::cin >> choice;
            if (choice == 1 && mercy_count > 0) {
                std::cout << "what would you like to buy?\n";
                std::cout << "1. Hero's Sword\n";
                std::cout << "2. health potion\n";
                std::cout << "3. leave\n";
                std::cin >> choice;
                if (choice == 1) {
                    if (player.gold >= 40) {
                        player.gold -= 40;
                        player.attack_range = {70, 90};
                        std::cout << "You bought a Hero's Sword!\n";
                        Sleep(2000);
                    }
                    else {
                        std::cout << "come back when you have money\n";
                        Sleep(2000);
                    }
                }
                else if (choice == 2) {
                    if (player.gold >= 10) {
                        player.gold -= 10;
                        player.health_potions += 5;
                    }
                    else {
                        std::cout << "come back when you have money\n";
                        Sleep(2000);
                    }
                }
                else if (choice == 3) {
                    std::cout << "NO WINDOW SHOPPING!!!\n";
                    Sleep(2000);
                }
            }
            else if (choice == 1 && mercy_count == 0) {
                std::cout << "1. Monster's Sword\n";
                std::cout << "2. health potion\n";
                std::cout << "3. leave\n";
                std::cin >> choice;
                if (choice == 1) {
                    if (player.gold >= 40) {
                        player.gold -= 40;
                        player.attack_range = {70, 90};
                        std::cout << "You bought a Monster's Sword!\n";
                        Sleep(2000);
                    }
                    else {
                        std::cout << "come back when you have money\n";
                        Sleep(2000);
                    }
                }
                else if (choice == 2) {
                    if (player.gold >= 10) {
                        player.gold -= 10;
                        player.health_potions += 5;
                    }
                    else {
                        std::cout << "come back when you have money\n";
                        Sleep(2000);
                    }
                }
                else if (choice == 3) {
                    std::cout << "NO WINDOW SHOPPING!!!\n";
                    Sleep(2000);
                }
            }
            else if (choice == 2) {
                std::cout << "come back when you have something to sell!!!\n";
                Sleep(2000);
            }
            else if (choice == 3) {
                std::cout << "NO WINDOW SHOPPING!!!\n";
                Sleep(2000);
            }
        }else if (action == 'e' && is_next_to_door(player_x, player_y, map) && map_filename == "maps/shop4.txt") {
            std::cout << "\033[32mYou opened the door and entered the town\033[0m\n";
            Sleep(2000);
            map_filename = "maps/town4.txt"; // Update map filename
            map = load_map_from_file(map_filename); // Load the next map
            player_x = 5; // Reset player position or set to desired spawn point
            player_y = 2; // Reset player position or set to desired spawn point
            save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
        } else if (action == 'e' && is_next_to_p(player_x, player_y, map) && map_filename == "maps/town4.txt") {
            std::cout << "are you ready to finish your journey? (this is the last time you will be asked)\n";
            Sleep(2000);
            std::cout << "1. yes\n";
            std::cout << "2. no\n";
            int choice;
            std::cin >> choice;
            if (choice == 1) {
                map_filename = "maps/level5.txt"; // Update map filename
                map = load_map_from_file(map_filename); // Load the next map
                player_x = 1; // Reset player position or set to desired spawn point
                player_y = 1; // Reset player position or set to desired spawn point
                save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
            }
            else {
                std::cout << "come back when you are ready to continue your journey\n";
                Sleep(2000);
            }
        } else if (action == 'e' && is_next_to_s(player_x, player_y, map) && map_filename == "maps/level5.txt") {
            std::cout << "Weird there isn't any enemies here\n";
            Sleep(2000);
            std::cout << "\nlooks safe to activate the switch\n";
            Beep(350, 300); // Make a beep sound
            Sleep(2000);
            for (auto& row : map) {
                std::replace(row.begin(), row.end(), 'S', ' ');
            }
            for (auto& row : map) {
                std::replace(row.begin(), row.end(), 'B', ' ');
            }
            Sleep(2000);
            std::cout << "the switch has broken and the door has opened\n";
            Sleep(2000);
        } else if (action == 'e' && is_next_to_p(player_x, player_y, map) && map_filename == "maps/level5.txt") {
            system("cls");
            std::cout << "oh thank you you saved me\n";
            Sleep(2000);
            std::cout << "i owe you my life\n";
            Sleep(2000);
            std::cout << "kidding\n";
            Sleep(2000);
            std::cout << "...\n";
            Sleep(1000);
            std::cout << "\033[31mTIME FOR YOU TO DIE\033[0m\n";
            Sleep(2000);
            system("cls");
            std::cout << "Prepare yourself for the final battle!\n";
            Sleep(2000);
            enemy.max_health = 1000;
            enemy.health = enemy.max_health;
            enemy.attack_range = {40, 60};
            enemy.name = "The Final Boss";
            if (fight_boss(player, enemy, mercy_count)) {
                std::cout << "You have defeated the final boss!\n";
                Sleep(2000);
                std::cout << "Congratulations, you have completed your journey!\n";
                Sleep(4000);
                if (mercy_count > 0) {
                    map_filename = "maps/ending1.txt"; // Update map filename
                }
                else {
                    map_filename = "maps/ending2.txt"; // Update map filename
                }
                map = load_map_from_file(map_filename); // Load the ending map
                player_x = 1; // Reset player position or set to desired spawn point
                player_y = 1; // Reset player position or set to desired spawn point
                save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
                //break;
            } else {
                std::cout << "You have been defeated by the final boss...\n";
                Sleep(2000);
                std::cout << "Game Over\n";
                Sleep(2000);
                break; // Exit the game loop if the player is defeated
            }
        }if (map_filename == "maps/ending1.txt") {
            system("cls");
            std::cout << "You have completed your journey and have been rewarded with a hero's welcome\n";
            Sleep(2000);
            std::cout << "Congratulations on your bravery and kindness\n";
            Sleep(2000);
            std::cout << "You are a true hero\n";
            Sleep(2000);
            std::cout << "You have been granted a place in history\n";
            Sleep(2000);
            std::cout << "as the hero who saved the town from the monster\n";
            Sleep(2000);
            std::cout << "Thank you for playing\n";
            Sleep(2000);
            break;
        }else if (map_filename == "maps/ending2.txt") {

            std::vector<std::string> ending_maps = {
                "maps/ending2.txt",
                "maps/ending21.txt",
                "maps/ending22.txt",
                "maps/ending23.txt",
                "maps/ending24.txt",
                "maps/ending25.txt",
                "maps/ending26.txt",
                "maps/ending27.txt",
                "maps/ending28.txt",
                "maps/ending29.txt"
            };

            for (const auto& map_file : ending_maps) {
                if (map_file == "maps/ending28.txt" || map_file == "maps/ending29.txt") {
                    flash_screen("YOU ARE A MONSTER", "YOU ARE A MONSTER", 100);
                }
                map = load_map_from_file(map_file);
                system("cls");
                ending_screen(map, terminal_width, terminal_height);
                Beep(50, 100);
                Sleep(2000);
                    
            }
            system("cls");
            std::cout << "\033[31mYou monster\033[0m\n";
            Sleep(2000);
            std::cout << "\033[31mYOU WILL BE REMEMBERED FOR ALL ETERNITY\033[0m\n";
            Sleep(2000);
            std::cout << "\033[31mAS THE HERO WHO KILLED EVERYTHING\033[0m\n";
            Sleep(2000);
            std::cout << "\033[31mFOR ALL ETERNITY\033[0m\n";
            Sleep(2000);
            std::cout << "\033[31mWHY DID YOU HAVE TO PLAY THIS GAME\033[0m\n";
            Sleep(2000);
            std::cout << "\033[31mYOU MONSTER\033[0m\n";
            Sleep(2000);
            std::cout << "Press any key to exit\n";
            _getch();
            //std::cin.get();
            break;
        }

    }

    save_game(player, enemy, mercy_count, player_x, player_y, map_filename); // Save game state
    end_screen(player); // Display end screen
    return 1;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file



