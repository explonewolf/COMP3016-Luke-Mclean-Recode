// functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <utility>
#include <vector>

class Character {
public:
    std::string name;
    int health;
    std::pair<int, int> attack_range; // attack_range as a pair of integers
    std::string weapon; // attribute for weapon
    int health_potions; // attribute for health potions
    int gold; // attribute for gold

    Character(std::string n, int h) : name(n), health(h), attack_range(5, 15), weapon("Fists"), health_potions(0), gold(0) {} // Initialize weapon

    int attack(Character& other);
    void mercy();
    bool is_alive();
    void use_health_potion();
};

void display_health(const Character& player, const Character& enemy);
void save_game(const Character& player, const Character& enemy, int mercy_count, int player_x, int player_y, const std::string& map_filename);
void load_game(Character& player, Character& enemy, int& mercy_count, int& player_x, int& player_y, std::string& map_filename);
void flash_screen(const std::string& player_action, const std::string& enemy_action, int damage);
void load_map(const std::string& filename);
void end_screen(const Character& player);
void player_attack(Character& player, Character& enemy);
void enemy_attack(Character& enemy, Character& player);
bool is_next_to_door(int player_x, int player_y, const std::vector<std::string>& map);
bool fight_M(Character& player, const std::string& map_filename);
#endif // FUNCTIONS_H
