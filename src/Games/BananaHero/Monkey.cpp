#include "Monkey.hpp"


template<typename T>
T sq(T v)
{
    return v*v;
}

void Monkey::position(sf::Vector2f pos)
{
    happy_.setPosition(pos);
    unhappy_.setPosition(pos);
    pos_ = pos;
}

bool Monkey::receive(const Banana& banana) const
{
    return (banana.position().y > pos_.y - 60) && (sq(banana.position().x - pos_.x) + sq(banana.position().y - pos_.y + 128) < sq(120));
}

void Monkey::setTextures(const sf::Texture& hap, const sf::Texture& unhap)
{
    happy_.setTexture(hap);
    unhappy_.setTexture(unhap);
}
