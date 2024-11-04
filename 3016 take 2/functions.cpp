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
#include <bits/algorithmfwd.h>

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
    file << "weapon " << player.weapon << " " << player.attack_range.first << " " << player.attack_range.second << "\n"; // line for weapon and attack range
    file << "health potions " << player.health_potions << "\n"; // Save health potions
    file << "gold " << player.gold << "\n"; // Save gold
}

void load_game(Character& player, Character& enemy, int& mercy_count, int& player_x, int& player_y, std::string& map_filename) {
    std::ifstream file("save.txt");
    if (file.is_open()) {
        std::string player_data, enemy_data, mercy_data, position_data, map_data, weapon_data;
        std::getline(file, player_data);
        std::getline(file, mercy_data);
        std::getline(file, enemy_data);
        std::getline(file, position_data);
        std::getline(file, map_data);
        std::getline(file, weapon_data); 

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

            // Weapon and attack range
            size_t weapon_end = weapon_data.find(' ', 7); // Find the end of the weapon name
            player.weapon = weapon_data.substr(7, weapon_end - 7); // Extract weapon name
            std::string attack_range_first_str = weapon_data.substr(weapon_end + 1, weapon_data.find(' ', weapon_end + 1) - weapon_end - 1);
            std::string attack_range_second_str = weapon_data.substr(weapon_data.rfind(' ') + 1);
            player.attack_range.first = std::stoi(attack_range_first_str);
            player.attack_range.second = std::stoi(attack_range_second_str);

            std::string potions_data, gold_data;
            std::getline(file, potions_data);
            std::getline(file, gold_data);

            // Health potions
            std::string potions_count_str = potions_data.substr(potions_data.rfind(' ') + 1);
            player.health_potions = std::stoi(potions_count_str);

            // Gold
            std::string gold_count_str = gold_data.substr(gold_data.rfind(' ') + 1);
            player.gold = std::stoi(gold_count_str);
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

bool fight_M(Character& player, const std::string& map_filename, int& mercy_count) {
    // Determine the enemy type based on the map
    std::string enemy_name;
    int hp = 0;
    if (map_filename == "maps/level1.txt") {
        // Randomly select an enemy type for level 1
        std::string enemies[] = {"Rat", "Squirrel", "Bunny", "Bird"};
        int random_index = rand() % 4; // Random number between 0 and 3
        enemy_name = enemies[random_index];
        hp = 30;
    } else if (map_filename == "maps/level2.txt") {
        std::string enemies[] = {"Skeleton", "Zombie", "Ghost", "Vampire"};
        int random_index = rand() % 4; // Random number between 0 and 3
        enemy_name = enemies[random_index];
        hp = 90;
    } else if (map_filename == "maps/level3.txt") {
        std::string enemies[] = {"Wolf", "Bear", "Fox", "Rabbit"};
        int random_index = rand() % 4; // Random number between 0 and 3
        enemy_name = enemies[random_index];
        hp = 140;
    } else if (map_filename == "maps/level4.txt") {
        std::string enemies[] = {"demon", "devil", "ghost", "zombie"};
        int random_index = rand() % 4; // Random number between 0 and 3
        enemy_name = enemies[random_index];
        hp = 200;
    } else {
        // Default enemy if map is not recognized
        enemy_name = "Unknown Creature";
        hp = 100;
    }
    Character enemy(enemy_name, hp);

    std::cout << "\n You encountered a " << enemy.name << "!\n";

    // Fight loop
    while (player.is_alive() && enemy.is_alive()) {
        display_health(player, enemy);
        std::string action;
        if (enemy.health < player.attack_range.second) {
            std::cout << "\033[33mMercy is available!\n\033[0m";
        }
        std::cout << "Choose action: (attack/mercy): ";
        std::cin >> action;

        if (action == "attack") {
            player_attack(player, enemy); // Player attacks
            if (enemy.is_alive()) {
                enemy_attack(enemy, player); // Enemy attacks back
            }
        } else if (action == "mercy") {
            if (enemy.health < player.attack_range.second) {
                std::cout << "You showed mercy and ended the fight!\n";
                mercy_count++;
                break; // End the fight
            }
            player.mercy();
        } else {
            std::cout << "Invalid action!\n";
        }
    }

    if (player.is_alive()) {
        std::cout << "You defeated the " << enemy.name << "!\n";
        if (map_filename == "maps/level1.txt") {
            int gold_earned = rand() % 3 + 3; // Random gold between 3 and 5
            player.gold += gold_earned;
            std::cout << "You earned " << gold_earned << " gold!\n";
        } else if (map_filename == "maps/level2.txt") {
            int gold_earned = rand() % 5 + 5; // Random gold between 5 and 10
            player.gold += gold_earned;
            std::cout << "You earned " << gold_earned << " gold!\n";
        } else if (map_filename == "maps/level3.txt") {
            int gold_earned = rand() % 7 + 7; // Random gold between 7 and 14
            player.gold += gold_earned;
            std::cout << "You earned " << gold_earned << " gold!\n";
        } else if (map_filename == "maps/level4.txt") {
            int gold_earned = rand() % 10 + 10; // Random gold between 10 and 20
            player.gold += gold_earned;
            std::cout << "You earned " << gold_earned << " gold!\n";
        }   
        return true;
    } else {
        std::cout << "You have been defeated by the " << enemy.name << "!\n";
        Sleep(3000);
        return false;
    }
}

void Character::use_health_potion() {
    if (health_potions > 0) {
        health += 30; // Heal for 30 HP
        health_potions--;
        std::cout << name << " used a health potion and healed for 30 HP!\n";
    } else {
        std::cout << "No health potions left!\n";
    }
}
int count_enemies(const std::vector<std::string>& map) {
    int remaining_enemies = 0;
    for (const auto& row : map) {
        remaining_enemies += std::count(row.begin(), row.end(), 'M');
    }
    return remaining_enemies;
}
bool fight_boss(Character& player, Character& enemy, int& mercy_count) {
    player.max_health = 1000;
    player.health = player.max_health;
    std::cout << "\n You encountered " << enemy.name << "!\n";
    
    while (player.is_alive() && enemy.is_alive()) {
        display_health(player, enemy);
        std::string action;
        if (enemy.health < player.attack_range.second) {
            std::cout << "\033[33mMercy is available!\n\033[0m";
        }
        std::cout << "Choose action: (attack/mercy): ";
        std::cin >> action;

        if (action == "attack") {
            player_attack(player, enemy); // Player attacks
            if (enemy.is_alive()) {
                enemy_attack(enemy, player); // Enemy attacks back
            }
            else {
                system("cls");
                std::cout << enemy.name << ":\033[31mYOU WILL REGRET KILLING ME\033[0m\n";
                Sleep(2000);
                std::cout << enemy.name << ":\033[31mYOU WILL PAY DEARLY FOR THIS\033[0m\n";
                Sleep(2000);
            }
        } else if (action == "mercy") {
            if (enemy.health < player.attack_range.second) {
                std::cout << "You showed mercy and ended the fight!\n";
                Sleep(2000);
                mercy_count++;
                std::cout << enemy.name << ": thank you for showing mercy\n";
                Sleep(2000);
                std::cout << enemy.name << ": you are a good person\n";
                Sleep(2000);
                std::cout << enemy.name << ": i owe you my life\n";
                Sleep(2000);
                break; // End the fight
            }
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

