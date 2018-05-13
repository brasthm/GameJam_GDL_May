#include <algorithm>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "Frog.hpp"


void Frog::update(const sf::Time& elapsedTime)
{
    if(isMoving())
    {
        auto movementTime = sf::seconds(56.f / speed);
        movementProgression_ += elapsedTime;
        float ratio = std::min(movementProgression_.asSeconds() / movementTime.asSeconds(), 1.f);
        position_ = ratio * nextPosition_ + (1-ratio) * lastPosition_;
        
        if(movementProgression_ >= movementTime)
        {
            lastPosition_ = nextPosition_;
            movementProgression_ = sf::Time::Zero;
        }
        
    }
    else
    {
        auto x = position_.x;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            x -= speed * elapsedTime.asSeconds();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            x += speed * elapsedTime.asSeconds();
        x = std::clamp(x, 32.f, 800.f-32.f);
        position({x, position_.y});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            nextPosition_.y -= 56;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && position_.y < 600-28)
            nextPosition_.y += 56;
        
    }
}
