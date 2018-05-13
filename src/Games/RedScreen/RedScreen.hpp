#ifndef ECRANROUGE_HPP
#define ECRANROUGE_HPP

#include "../Game.hpp"

class RedScreen : public Game
{
    public:
        explicit RedScreen(sf::RenderTarget& window, DJ& dj) : Game{window, dj} {}
    
        bool computeFrame(const sf::Time& elapsedTime, int& score) override { return true; };
        void drawState(sf::Sprite &countdown) const;
};


#endif //ECRANROUGE_HPP
