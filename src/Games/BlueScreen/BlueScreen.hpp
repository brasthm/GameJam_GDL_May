#ifndef ECRANBLEU_HPP
#define ECRANBLEU_HPP

#include "../Game.hpp"

class BlueScreen : public Game
{
    public:
        explicit BlueScreen(sf::RenderTarget& window, DJ& dj) : Game{window, dj} {}
    
        bool computeFrame(const sf::Time& elapsedTime, int& score) override { return true; };
        void drawState(sf::Sprite &countdown) const;
};


#endif //ECRANBLEU_HPP
