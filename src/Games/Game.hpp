#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

class Game
{
    public:
        explicit Game(sf::RenderTarget& window) : window_{window} {}
        virtual ~Game() = default;
        
        virtual bool computeFrame(const sf::Time& elapsedTime, int& score) = 0;
        virtual void drawState() const = 0;
    
    protected:
        sf::RenderTarget& window_;
};

#endif //GAME_HPP
