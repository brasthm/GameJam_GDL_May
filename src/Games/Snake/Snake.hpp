#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "../Game.hpp"
#include "BodyPart.hpp"


class Snake : public Game
{
    public:
        explicit Snake(sf::RenderTarget& window);

        bool computeFrame(const sf::Time& elapsedTime, int& score) override;
        void drawState() const override;
        
        void randomApple();
        void randomSnake(size_t bodyPartMin = 1, size_t bodyPartMax = 10);

    private:
        std::vector<BodyPart> snake_;
        orientation_t currentOrientation_;
        orientation_t savedOrientation_;
        sf::Sprite sprites_[3][4];
        sf::Sprite appleSprite_;
        std::vector<sf::Texture> textures_;
        sf::Time cycleProgression_;
        sf::Sprite bg_;
        
        const sf::Time cycleTime = sf::milliseconds(100);
        
        bool lose = false;
        sf::Vector2f applePos_;
        bool appleAte = false;
};


#endif //SNAKE_HPP
