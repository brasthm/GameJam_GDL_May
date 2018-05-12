#ifndef BANANAHERO_HPP
#define BANANAHERO_HPP

#include "../Game.hpp"
#include "Monkey.hpp"
#include <SFML/Graphics/Texture.hpp>

class BananaHero : public Game
{
    public:
        explicit BananaHero(sf::RenderWindow& window);
    
        void computeFrame(const sf::Time& elapsedTime) override;
        void drawState() const override;
    
    private:
        std::vector<Banana> bananas_;
        Monkey monkey_;
        static constexpr float monkeySpeed = 200;
        std::vector<sf::Texture> textures_;
        sf::Time spawnDelay_;
        sf::Time spawnProgression_;
};


#endif //BANANAHERO_HPP
