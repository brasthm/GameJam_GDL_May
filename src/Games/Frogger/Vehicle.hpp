#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Frog.hpp"

class Vehicle
{
    public:
        Vehicle(sf::Vector2f pos, const sf::Texture& texture, float speed, Frog* frog);
        void update(const sf::Time& elapsedTime);
        const sf::Sprite& sprite() const { return sprite_; }
        bool isFrogOn(const Frog& frog) const {
            return sprite_.getGlobalBounds().contains(frog.position());
        }
        bool isOut() const { return !sprite_.getGlobalBounds().intersects({0,0,800,600}) && speed_ * sprite_.getPosition().x > 0; }
            
    private:
        sf::Sprite sprite_;
        Frog* frog_ = nullptr;
        float speed_;
};


#endif //VEHICLE_HPP
