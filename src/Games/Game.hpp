#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "../DJ.hpp"

class Game
{
    public:
        explicit Game(sf::RenderTarget& window, DJ& dj) : window_{window}, Dj_(dj) {}
        virtual ~Game() = default;
        
        virtual bool computeFrame(const sf::Time& elapsedTime, int& score) = 0;
        virtual void drawState(sf::Sprite &countdown) const = 0;
    
    protected:
        sf::RenderTarget& window_;
		DJ& Dj_;
};

#endif //GAME_HPP
