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
    return map[y][x] != 'T' && map[y][x] != 'P' && map[y][x] != 'D'; // Check if the target position is not a tree, person, or door
}

void display_header(int terminal_width) {
    std::string instructions[] = {
        "Instructions:",
        "Use 'w', 'a', 's', 'd' to move.",
        "Press 'e' to interact.",
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

    // Load map from file
    std::vector<std::string> map = load_map_from_file("maps/Tutorial.txt");

    int player_x = 1, player_y = 1; // Initial player position
    int mercy_count = 0; // Initialize mercy_count

    // Declare player and enemy outside the loop
    Character player("@", 100);
    Character enemy("Tutorial Enemy", 50);

    while (true) {
        system("cls"); // Clear the console (Windows-specific)
        int terminal_width = get_terminal_width();
        int terminal_height = get_terminal_height();

        display_header(terminal_width);
        display_map(map, player_x, player_y, terminal_width, terminal_height);

        std::cout << "Move (w/a/s/d) or interact (e): ";
        char action = _getch(); // Use _getch() to capture key press without enter

        int new_x = player_x;
        int new_y = player_y;

        if (action == 'w' && player_y > 0) new_y--;
        else if (action == 's' && player_y < map.size() - 1) new_y++;
        else if (action == 'a' && player_x > 0) new_x--;
        else if (action == 'd' && player_x < map[0].size() - 1) new_x++;

        // Check if the new position is valid (not a tree, person, or door)
        if (can_move_to(new_x, new_y, map)) {
            player_x = new_x;
            player_y = new_y;
        }

        if (action == 'e' && is_next_to_p(player_x, player_y, map)) {
            std::cout << "Starting tutorial fight!\n";
            
            // Start tutorial fight
            int enemy_max_health = enemy.health;

            load_game(player, enemy, mercy_count); // Load game state

            while (player.is_alive() && enemy.is_alive()) {
                display_health(player, enemy);
                std::string action;
                if (enemy.health < 0.2 * enemy_max_health) {
                    std::cout << "\033[33mMercy is available!\n\033[0m";
                }
                std::cout << "Choose action: (attack/mercy): ";
                std::cin >> action;
                

                if (action == "attack") {
                    player_attack(player, enemy); // Player attacks
                    enemy_attack(enemy, player); // Enemy attacks back
                } else if (action == "mercy") {
                    if (enemy.health < 0.2 * enemy_max_health) { // Check if enemy's health is below 20%
                        std::cout << "You showed mercy and ended the fight!\n";
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
                std::cout << "You defeated the enemy and received a Wooden Sword!\n";
                // Update player's attack range
                player.attack_range = {15, 20};

                // Remove 'P' from the map
                for (auto& row : map) {
                    std::replace(row.begin(), row.end(), 'P', ' ');
                }
            } else {
                std::cout << "You have been defeated!\n";
                break; // Exit the game loop if the player is defeated
            }
        } else if (action == 'e' && is_next_to_door(player_x, player_y, map)) {
            std::cout << "You opened the door and entered the next area!\n";
            map = load_map_from_file("maps/level1.txt"); // Load the next map
            player_x = 1; // Reset player position or set to desired spawn point
            player_y = 1; // Reset player position or set to desired spawn point
}
    }

    save_game(player, enemy, mercy_count); // Save game state
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



