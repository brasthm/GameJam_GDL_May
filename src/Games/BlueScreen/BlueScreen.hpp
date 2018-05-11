#ifndef ECRANBLEU_HPP
#define ECRANBLEU_HPP

#include "../Game.hpp"

class BlueScreen : public Game
{
    public:
        explicit BlueScreen(sf::RenderWindow& window) : Game{window} {}
    
        void computeFrame(const sf::Time& elapsedTime) override {};
        void drawState() const override;
};


#endif //ECRANBLEU_HPP
