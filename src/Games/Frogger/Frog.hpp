#ifndef FROG_HPP
#define FROG_HPP


#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>


class Frog
{
    public:
        sf::Vector2f position() const { return position_; }
        void position(sf::Vector2f pos) {
            pos.x = std::clamp(pos.x, 32.f, 800.f-32.f);
            position_ = lastPosition_ = nextPosition_ = pos; }
        bool isMoving() const { return lastPosition_ != nextPosition_; }
        void update(const sf::Time& elapsedTime);
    
    private:
        sf::Vector2f position_;
        sf::Vector2f lastPosition_;
        sf::Vector2f nextPosition_;
        sf::Time movementProgression_;
        static constexpr float speed = 200;
};


#endif //FROG_HPP
