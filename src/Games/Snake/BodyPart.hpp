#ifndef BODYPART_HPP
#define BODYPART_HPP


#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include "../../DirectionalSprite.h"

class BodyPart
{
    public:
        enum type_t { HEAD, BODY, TAIL };
        
        BodyPart(type_t t, sf::Vector2f pos, orientation_t ori);
        
        void update(const sf::Time& elapsedTime);
        
        type_t type() const { return type_; }
        void type(type_t new_type) { type_ = new_type; }
    
        sf::Vector2f position() const { return position_; }
        void move(orientation_t orientation);
        
        sf::Vector2f nextPosition() const { return nextPosition_; }
        sf::Vector2f lastPosition() const { return lastPosition_; }
    
        orientation_t orientation() const { return orientation_; }
        void orientation(orientation_t new_orientation) { orientation_ = new_orientation; }
        
    private:
        type_t type_;
        sf::Vector2f position_;
        sf::Vector2f lastPosition_;
        sf::Vector2f nextPosition_;
        orientation_t orientation_;
        sf::Time movementProgression_;
        
        static constexpr float tileSize = 40;
        const sf::Time movementTime = sf::milliseconds(100);
};

#endif //BODYPART_HPP
