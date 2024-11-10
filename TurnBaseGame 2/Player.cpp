#include "Player.h"
#include <iostream>

Player::Player(const std::string& name) : name(name), health(100), currentAttackIndex(0)
{
    // Create attacks 

    attackOptions.push_back("Slash");
    attackOptions.push_back("Kick");
    attackOptions.push_back("Fireball");

    //More? if time
}

void Player::selectAttack(int index)
{
    if (index >= 0 && index < attackOptions.size())
    {
        currentAttackIndex = index;
    }
    else
    {
        std::cout << "error\n"; //tbc
    }
}


void Player::performAttack(Enemy& enemy)
{
    if (currentAttackIndex < 0 || currentAttackIndex >= attackOptions.size())
    {
        std::cout << "No attack sleected\n";
        return;
    }

    std::cout << name << " attacks " << enemy.getName() << " with " << attackOptions[currentAttackIndex] << " Helath is now " << enemy.getHealth() << "!\n"; //golbin has been killed infinate
    enemy.receiveDamage(rand()%30); // Example damage (tbc)
}


void Player::receiveDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0; // Prevent negative health
}



bool Player::isAlive() const
{
    return health > 0;
}



int Player::getHealth() const
{
    return health;
}



std::string Player::getName() const
{
    return name;
}


std::vector<std::string> Player::getAttackOptions() const
{
    return attackOptions;
}