#include <random>
#include "Banana.hpp"


Banana::Banana(Banana::type_t type) : type_{type}
{
    static std::random_device rd;
    static std::default_random_engine gen(rd());
    std::uniform_int_distribution<int> xDistrib(40, 760);
    
    pos_.x = xDistrib(gen);
    pos_.y = -40;
}

void Banana::update(const sf::Time& elapsedTime)
{
    if(type_ == NORMAL || type_ == ROTTEN)
        pos_.y += normalSpeed * elapsedTime.asSeconds();
    else 
        pos_.y += doubleSpeed * elapsedTime.asSeconds();
}
