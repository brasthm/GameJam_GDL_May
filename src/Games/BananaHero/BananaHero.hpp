#ifndef BANANAHERO_HPP
#define BANANAHERO_HPP

#include "../Game.hpp"
#include "Monkey.hpp"
#include <SFML/Graphics/Texture.hpp>

class BananaHero : public Game
{
    public:
        explicit BananaHero(sf::RenderTarget& window, DJ& dj);
    
        bool computeFrame(const sf::Time& elapsedTime, int& score) override;
        void drawState(sf::Sprite &countdown) const override;
        
        void generateBanana();
    
    private:
        std::vector<Banana> bananas_;
        Monkey monkey_;
        static constexpr float monkeySpeed = 800;
        std::vector<sf::Texture> textures_;
        sf::Time spawnDelay_;
        sf::Time spawnProgression_;
        float x;
        sf::Sprite bg_;
};


#endif //BANANAHERO_HPP
