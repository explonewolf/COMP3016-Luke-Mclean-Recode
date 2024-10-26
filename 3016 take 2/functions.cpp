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

void save_game(const Character& player, const Character& enemy, const int mercy_count) {
    std::ofstream file("save.txt");
    file << player.name << " " << player.health << "\n";
    file << "mercy count " << mercy_count <<"\n";
    file << enemy.name << " " << enemy.health << "\n";
    
}

void load_game(Character& player, Character& enemy, int& mercy_count) {
    std::ifstream file("save.txt");
    if (file.is_open()) {
        std::string player_data, enemy_data, mercy_data;
        std::getline(file, player_data);
        std::getline(file, mercy_data);
        std::getline(file, enemy_data);
        player.name = player_data.substr(0, player_data.find(' '));
        player.health = std::stoi(player_data.substr(player_data.find(' ') + 1));
        mercy_count = std::stoi(mercy_data.substr(mercy_data.find(' ') + 1));
        enemy.name = enemy_data.substr(0, enemy_data.find(' '));
        enemy.health = std::stoi(enemy_data.substr(enemy_data.find(' ') + 1));
    } else {
        player = Character("@", 100);
        enemy = Character("Tutorial Enemy", 50);
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
