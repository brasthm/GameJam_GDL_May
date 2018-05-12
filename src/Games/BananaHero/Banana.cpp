#include <random>
#include "Banana.hpp"


Banana::Banana(Banana::type_t type, float x) : type_{type}
{
    pos_.x = x;
    pos_.y = -40;
}

void Banana::update(const sf::Time& elapsedTime)
{
    if(type_ == NORMAL || type_ == ROTTEN)
        pos_.y += normalSpeed * elapsedTime.asSeconds();
    else 
        pos_.y += doubleSpeed * elapsedTime.asSeconds();
}
