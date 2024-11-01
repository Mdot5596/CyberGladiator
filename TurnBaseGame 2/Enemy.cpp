#include "Enemy.h"
#include <iostream>

Enemy::Enemy(const std::string& name) : name(name), health(100) {}

void Enemy::receiveDamage(int damage) 
{
    health -= damage;
    if (health < 0) health = 0; // Prevents going negative health
}

bool Enemy::isAlive() const 
{
    return health > 0;
}

int Enemy::getHealth() const
{
    return health;
}

std::string Enemy::getName() const 
{
    return name;
}
