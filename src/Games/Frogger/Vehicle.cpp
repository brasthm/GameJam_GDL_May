#include "Vehicle.hpp"


Vehicle::Vehicle(sf::Vector2f pos, const sf::Texture& texture, float speed, Frog* frog)
{
    sprite_.setPosition(pos);
    sprite_.setTexture(texture);
    sprite_.setOrigin(sprite_.getGlobalBounds().width/2, 28);
    speed_ = speed;
    if(speed < 0)
        sprite_.scale(-1, 1);
    frog_ = frog;
}

void Vehicle::update(const sf::Time& elapsedTime)
{
    auto pos = sprite_.getPosition();
    pos.x += speed_ * elapsedTime.asSeconds();
    
    sprite_.setPosition(pos);
}
