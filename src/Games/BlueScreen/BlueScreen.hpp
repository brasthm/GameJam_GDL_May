#ifndef ECRANBLEU_HPP
#define ECRANBLEU_HPP

#include "../Game.hpp"

class BlueScreen : public Game
{
    public:
        explicit BlueScreen(sf::RenderTarget& window) : Game{window} {}
    
        bool computeFrame(const sf::Time& elapsedTime, int& score) override { return true; };
        void drawState() const override;
};


#endif //ECRANBLEU_HPP
