#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <windows.h>
#include "functions.h"
#include <vector>

int Character::attack(Character& other) {
    std::cout << name << " is attacking " << other.name << std::endl; // Debug statement
    int damage = rand() % (attack_range.second - attack_range.first + 1) + attack_range.first; // Use attack_range for damage
    other.health -= damage; // Apply damage to the opponent's health

    // Create message for the attack
    std::string action_message = name + " attacks " + other.name + " for " + std::to_string(damage) + " damage!";
    
    // Flash the screen with the action message
    flash_screen(action_message, "", damage); // Only show the attack action

    //Print the damage dealt
    //std::cout << action_message << "\n"; // Ensure this line is present to display damage dealt

    return damage; // Return the damage dealt
}

void Character::mercy() {
    std::cout << name << " shows mercy!\n";
}

bool Character::is_alive() {
    return health > 0;
}

void display_health(const Character& player, const Character& enemy) {
    std::cout << player.name << " HP: " << player.health << " | " << enemy.name << " HP: " << enemy.health << "\n";
}

void save_game(const Character& player, const Character& enemy, const int mercy_count, int player_x, int player_y, const std::string& map_filename) {
    std::ofstream file("save.txt");
    file << player.name << " " << player.health << "\n";
    file << "mercy count " << mercy_count << "\n";
    file << enemy.name << " " << enemy.health << "\n";
    file << "player position " << player_x << " " << player_y << "\n";
    file << "map " << map_filename << "\n";
}

void load_game(Character& player, Character& enemy, int& mercy_count, int& player_x, int& player_y, std::string& map_filename) {
    std::ifstream file("save.txt");
    if (file.is_open()) {
        std::string player_data, enemy_data, mercy_data, position_data, map_data;
        std::getline(file, player_data);
        std::getline(file, mercy_data);
        std::getline(file, enemy_data);
        std::getline(file, position_data);
        std::getline(file, map_data);

        try {
            // Player health
            player.name = player_data.substr(0, player_data.find(' '));
            std::string player_health_str = player_data.substr(player_data.find(' ') + 1);
            std::cout << "Player health string: " << player_health_str << std::endl;
            player.health = std::stoi(player_health_str);

            // Mercy count
            std::string mercy_count_str = mercy_data.substr(mercy_data.rfind(' ') + 1);
            std::cout << "Mercy count string: " << mercy_count_str << std::endl;
            mercy_count = std::stoi(mercy_count_str);

            // Enemy health
            enemy.name = enemy_data.substr(0, enemy_data.rfind(' '));
            std::string enemy_health_str = enemy_data.substr(enemy_data.rfind(' ') + 1);
            std::cout << "Enemy health string: " << enemy_health_str << std::endl;
            enemy.health = std::stoi(enemy_health_str);

            // Player position
            size_t first_space = position_data.find(' ', position_data.find(' ') + 1);
            std::string player_x_str = position_data.substr(first_space + 1, position_data.find(' ', first_space + 1) - first_space - 1);
            std::cout << "Player X position string: " << player_x_str << std::endl;
            player_x = std::stoi(player_x_str);

            std::string player_y_str = position_data.substr(position_data.rfind(' ') + 1);
            std::cout << "Player Y position string: " << player_y_str << std::endl;
            player_y = std::stoi(player_y_str);

            // Map filename
            map_filename = map_data.substr(map_data.find(' ') + 1);
        } catch (const std::exception& e) {
            std::cerr << "Error loading game state: " << e.what() << std::endl;
            Sleep(5000);
            // Set default values or handle the error as needed
        }
    } else {
        player = Character("@", 100);
        enemy = Character("Tutorial Enemy", 50);
        player_x = 1;
        player_y = 1;
        map_filename = "maps/Tutorial.txt";
    }
}

void flash_screen(const std::string& enemy_action, const std::string& player_action, int damage) {
    // Flash the screen red
    std::cout << "\033[41m"; // ANSI escape code for red background
    std::cout << std::string(100, '\n'); // Fill the screen with new lines to simulate a flash
    system("cls"); // Clear the console (Windows-specific)

    std::cout << "\033[0m"; // Reset to default colors
    std::cout << std::string(100, '\n'); // Fill the screen with new lines to simulate a flash
    system("cls"); // Clear the console (Windows-specific)

    // Display the last actions
    //std::cout << "\033[31m" << enemy_action << "\033[0m\n"; // Show enemy's attack in red
    
}

void load_map(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
}

void end_screen(const Character& player) {
    if (player.health > 0) {
        std::cout << "You are a Hero!\n";
    } else {
        std::cout << "You are a Monster!\n";
    }
}

void player_attack(Character& player, Character& enemy) {
    //int damage = player.attack(enemy);
    //std::cout << player.name << " attacks " << enemy.name << " for " << damage << " damage!\n";
}

void enemy_attack(Character& enemy, Character& player) {
    int damage1 = player.attack(enemy);
    if (enemy.is_alive()) {
        int damage = enemy.attack(player);
        std::cout << "\033[31m" << enemy.name << " attacks " << player.name << " for " << damage << " damage!\033[0m\n";
        std::cout << player.name << " attacks " << enemy.name << " for " << damage1 << " damage!\n";
    }
}
bool is_next_to_door(int player_x, int player_y, const std::vector<std::string>& map) {
    return (player_x > 0 && map[player_y][player_x - 1] == 'D') ||
           (player_x < map[0].size() - 1 && map[player_y][player_x + 1] == 'D') ||
           (player_y > 0 && map[player_y - 1][player_x] == 'D') ||
           (player_y < map.size() - 1 && map[player_y + 1][player_x] == 'D');
}

bool fight_M(Character& player, const std::string& map_filename) {
    // Determine the enemy type based on the map
    std::string enemy_name;
    if (map_filename == "maps/level1.txt") {
        // Randomly select an enemy type for level 1
        std::string enemies[] = {"Rat", "Squirrel", "Bunny", "Bird"};
        int random_index = rand() % 4; // Random number between 0 and 3
        enemy_name = enemies[random_index];
    } else {
        // Default enemy if map is not recognized
        enemy_name = "Unknown Creature";
    }

    // Create the enemy character
    Character enemy(enemy_name, 30);

    std::cout << "You encountered a " << enemy.name << "!\n";

    // Fight loop
    while (player.is_alive() && enemy.is_alive()) {
        display_health(player, enemy);
        std::string action;
        std::cout << "Choose action: (attack/mercy): ";
        std::cin >> action;

        if (action == "attack") {
            player_attack(player, enemy); // Player attacks
            if (enemy.is_alive()) {
                enemy_attack(enemy, player); // Enemy attacks back
            }
        } else if (action == "mercy") {
            player.mercy();
        } else {
            std::cout << "Invalid action!\n";
        }
    }

    if (player.is_alive()) {
        std::cout << "You defeated the " << enemy.name << "!\n";
        return true;
        
    } else {
        std::cout << "You have been defeated by the " << enemy.name << "!\n";
        Sleep(3000);
        return false;
    }
}

