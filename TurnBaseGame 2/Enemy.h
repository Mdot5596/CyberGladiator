#pragma once
#include <string>

class Enemy 
 {
public:
    Enemy(const std::string& name);

    void receiveDamage(int damage);  // Reduce health when taking damage
    bool isAlive() const;           // Check if the enemy is still alive
    int getHealth() const;          // Get current health
    std::string getName() const;   // Get enemy name

private:
    std::string name;
    int health;                   //enemy Hhealth
};
