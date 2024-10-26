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
    std::pair<int, int> attack_range; // Add attack_range as a pair of integers

    Character(std::string n, int h) : name(n), health(h), attack_range(5, 15) {} // Initialize attack_range

    int attack(Character& other);
    void mercy();
    bool is_alive();
};

void display_health(const Character& player, const Character& enemy);
void save_game(const Character& player, const Character& enemy, int mercy_count);
void load_game(Character& player, Character& enemy, int& mercy_count);
void flash_screen(const std::string& player_action, const std::string& enemy_action, int damage);
void load_map(const std::string& filename);
void end_screen(const Character& player);
void player_attack(Character& player, Character& enemy);
void enemy_attack(Character& enemy, Character& player);
bool is_next_to_door(int player_x, int player_y, const std::vector<std::string>& map);
#endif // FUNCTIONS_H
