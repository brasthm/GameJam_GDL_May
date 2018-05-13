#ifndef FROGGER_HPP
#define FROGGER_HPP

#include "../Game.hpp"
#include "Frog.hpp"
#include "Vehicle.hpp"
#include "Trunk.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


class Frogger : public Game
{
    public:
        explicit Frogger(sf::RenderTarget& window_, DJ& dj);
        
        bool computeFrame(const sf::Time& elapsedTime, int& score) override;
        void drawState(sf::Sprite& countdown) const override;
        
    private:
        std::vector<sf::Texture> textures_;
        sf::RectangleShape water_;
        sf::Sprite ground_;
        sf::Sprite frogSprite_;
        Frog frog_;
        std::vector<std::vector<Trunk>> trunks_;
        std::vector<sf::Time> trunkSpawnProgression_;
        std::vector<sf::Time> trunkSpawnTime_;
        std::vector<std::vector<Vehicle>> vehicles_;
        std::vector<sf::Time> vehicleSpawnProgression_;
        std::vector<sf::Time> vehicleSpawnTime_;
        
        bool lose_ = false;
        
        float minY;
        
        int currentWaterTexture_;
        sf::Time waterAnimationProgression_;
        static const sf::Time waterAnimationTime;
};


#endif //FROGGER_HPP
