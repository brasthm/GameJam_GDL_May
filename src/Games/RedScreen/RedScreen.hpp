#ifndef ECRANROUGE_HPP
#define ECRANROUGE_HPP

#include "../Game.hpp"

class RedScreen : public Game
{
    public:
        explicit RedScreen(sf::RenderTarget& window) : Game{window} {}
    
        bool computeFrame(const sf::Time& elapsedTime, int& score) override { return true; };
        void drawState() const override;
};


#endif //ECRANROUGE_HPP
