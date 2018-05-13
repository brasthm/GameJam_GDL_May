#ifndef MONKEY_HPP
#define MONKEY_HPP

#include "Banana.hpp"
#include <SFML/Graphics/Sprite.hpp>

class Monkey
{
    public:
        Monkey() : ishap_{true} { happy_.setOrigin(80, 60); unhappy_.setOrigin(80, 60); }
        void setTextures(const sf::Texture& hap, const sf::Texture& unhap);
        sf::Vector2f position() const { return pos_; }
        void position(sf::Vector2f pos);
        const sf::Sprite& sprite() const { return ishap_ ? happy_ : unhappy_; }
        void happy(bool b) { ishap_ = b; }
    
        bool receive(const Banana&) const;
        
    private:
        sf::Vector2f pos_;
        bool ishap_;
        sf::Sprite happy_, unhappy_;
};


#endif //MONKEY_HPP
