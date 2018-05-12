#ifndef BANANA_HPP
#define BANANA_HPP


#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>


class Banana
{
    public:
        enum type_t { NORMAL, DOUBLE, ROTTEN, DOUBLEROTTEN };
    
        explicit Banana(type_t type);
        
        type_t type() const { return type_; }
        
        sf::Vector2f position() const  { return pos_; }
    
        void update(const sf::Time& elapsedTime);
            
    private:
        type_t type_;
        sf::Vector2f pos_;
        static constexpr float normalSpeed = 100;
        static constexpr float doubleSpeed = 150;
};


#endif //BANANA_HPP
