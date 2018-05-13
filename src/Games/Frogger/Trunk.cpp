#include "Trunk.hpp"


Trunk::Trunk(sf::Vector2f pos, const sf::Texture& texture, float speed, Frog* frog)
{
    position_ = pos;
    sprite_.setPosition(pos);
    sprite_.setTexture(texture);
    sprite_.setOrigin(128, 28);
    speed_ = speed;
    frog_ = frog;
}

bool Trunk::isFrogOn(const Frog& frog) const
{
    return sprite_.getGlobalBounds().contains(frog.position());
}

void Trunk::update(const sf::Time& elapsedTime)
{
    auto pos = sprite_.getPosition();
    pos.x += speed_ * elapsedTime.asSeconds();
    
    if(frog_ != nullptr && isFrogOn(*frog_) && !frog_->isMoving())
    {
        auto frogPos = frog_->position();
        frogPos.x += speed_ * elapsedTime.asSeconds();
        frog_->position(frogPos);
    }
    
    sprite_.setPosition(pos);
    position_ = pos;
}
