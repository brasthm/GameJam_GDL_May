#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "../Game.hpp"
#include "BodyPart.hpp"


class Snake : public Game
{
    public:
        explicit Snake(sf::RenderWindow& window);

        void computeFrame(const sf::Time& elapsedTime) override;
        void drawState() const override;

    private:
        std::vector<BodyPart> snake_;
        orientation_t currentOrientation_;
        orientation_t savedOrientation_;
        sf::Sprite sprites_[3][4];
        std::vector<sf::Texture> textures_;
        sf::Time cycleProgression_;
        
        const sf::Time cycleTime = sf::milliseconds(100);
        
        bool lose = false;
};


#endif //SNAKE_HPP
