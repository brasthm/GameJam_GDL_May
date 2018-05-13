#ifndef TRUNK_HPP
#define TRUNK_HPP

#include <SFML/Graphics/Sprite.hpp>
#include "Frog.hpp"

class Trunk
{
    public:
        Trunk(sf::Vector2f pos, const sf::Texture& texture, float speed, Frog* frog);
        void update(const sf::Time& elapsedTime);
        bool isFrogOn(const Frog& frog) const;
        sf::Vector2f position() const { return position_; }
        const sf::Sprite& sprite() const { return sprite_; }
        bool isOut() const { return !sprite_.getGlobalBounds().intersects({0,0,800,600}) && speed_ * position_.x > 0; }
    
    private:
        float speed_;
        Frog* frog_ = nullptr;
        sf::Vector2f position_;
        sf::Sprite sprite_;
};


#endif //TRUNK_HPP
