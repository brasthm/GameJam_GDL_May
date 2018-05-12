#include <algorithm>
#include "BodyPart.hpp"


const sf::Time BodyPart::movementTime = sf::milliseconds(100);

BodyPart::BodyPart(BodyPart::type_t t, sf::Vector2f pos, orientation_t ori) :
    type_{t},
    position_{pos}, lastPosition_{pos}, nextPosition_{pos},
    orientation_{ori},
    movementProgression_{sf::Time::Zero}
{}

void BodyPart::move(orientation_t orientation)
{
    switch(orientation)
    {
        case UP:
            nextPosition_ += {0, -BodyPart::tileSize};
            break;
            
        case DOWN:
            nextPosition_ += {0, BodyPart::tileSize};
            break;
            
        case RIGHT:
            nextPosition_ += {BodyPart::tileSize, 0};
            break;
            
        case LEFT:
            nextPosition_ += {-BodyPart::tileSize, 0};
            break;
    }
}

void BodyPart::update(const sf::Time& elapsedTime)
{
    if(nextPosition_ != lastPosition_)
    {
        movementProgression_ = std::min(movementProgression_+elapsedTime, movementTime);
        float progression = movementProgression_.asSeconds() / movementTime.asSeconds();

        position_.x = nextPosition_.x * progression + lastPosition_.x * (1-progression);
        position_.y = nextPosition_.y * progression + lastPosition_.y * (1-progression);
        
        if(movementProgression_ == movementTime)
        {
            lastPosition_ = nextPosition_;
            movementProgression_ = sf::Time::Zero;
        }
    }
}
