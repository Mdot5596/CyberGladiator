#pragma once
#include <vector>
#include <string>
#include "Enemy.h" 

class Player
{

public:

    Player(const std::string& name);

    void selectAttack(int index);          // Select an attack from indec
    void performAttack(Enemy& enemy);     // Perform the attack on enemy
    void receiveDamage(int damage);      // Reduce health when taking damage
    bool isAlive() const;               // Checks if players alive

    int getHealth() const;             // Get current health
    std::string getName() const;      // Get player name


    std::vector<std::string> getAttackOptions() const; //Displays indx to temnial


private:

    std::string name;
    int health;
    std::vector<std::string> attackOptions; // List of attack names
    int currentAttackIndex;                // Index of selected attack

};