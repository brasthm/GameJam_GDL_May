#ifndef ECRANROUGE_HPP
#define ECRANROUGE_HPP

#include "../Game.hpp"

class RedScreen : public Game
{
    public:
        explicit RedScreen(sf::RenderWindow& window) : Game{window} {}
    
        void computeFrame(const sf::Time& elapsedTime) override {};
        void drawState() const override;
};


#endif //ECRANROUGE_HPP
