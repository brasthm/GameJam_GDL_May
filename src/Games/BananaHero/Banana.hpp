#ifndef BANANA_HPP
#define BANANA_HPP


#include <SFML/System/Vector2.hpp>


class Banana
{
    public:
        enum type_t { NORMAL, DOUBLE, ROTTEN, DOUBLEROTTEN };
    
        explicit Banana(type_t type);
        
        sf::Vector2f position() const  { return pos_; }
            
    private:
        type_t type_;
        sf::Vector2f pos_;
};


#endif //BANANA_HPP
