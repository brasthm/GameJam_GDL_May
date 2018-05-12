#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

class Game
{
    public:
        explicit Game(sf::RenderWindow& window) : window_{window} {}
        virtual ~Game() = default;
        
        virtual void computeFrame(const sf::Time& elapsedTime) = 0;
        virtual void drawState() const = 0;
    
    protected:
        sf::RenderWindow& window_;
};

#endif //GAME_HPP
