#include <iostream>
#include "Frogger.hpp"
#include "../../constants.hpp"
#include "../../utilities.hpp"


const sf::Time Frogger::waterAnimationTime = sf::milliseconds(500);

Frogger::Frogger(sf::RenderTarget& window_, DJ& dj) : Game(window_, dj), frog_{dj}
{
    textures_.resize(9);
    textures_[0].loadFromFile(location "img/frogger/water1.png");
    textures_[0].setRepeated(true);
    textures_[1].loadFromFile(location "img/frogger/water2.png");
    textures_[1].setRepeated(true);
    textures_[2].loadFromFile(location "img/frogger/water3.png");
    textures_[2].setRepeated(true);
    textures_[3].loadFromFile(location "img/frogger/water4.png");
    textures_[3].setRepeated(true);
    textures_[4].loadFromFile(location "img/frogger/ground.png");
    textures_[5].loadFromFile(location "img/frogger/frog.png");
    textures_[6].loadFromFile(location "img/frogger/trunk.png");
    textures_[7].loadFromFile(location "img/frogger/car.png");
    textures_[8].loadFromFile(location "img/frogger/truck.png");
    water_.setTexture(&textures_[0]);
    ground_.setTexture(textures_[4]);
    currentWaterTexture_ = 0;
    water_.setSize({800, 600});
    water_.setTextureRect({0, 0, 800, 600});
    frogSprite_.setTexture(textures_[5]);
    frogSprite_.setOrigin(32, 28);
    frog_.position({400, 600-28});
    trunks_.resize(4);
    trunkSpawnProgression_.resize(4);
    trunkSpawnTime_.push_back(sf::Time::Zero);
    trunkSpawnTime_.push_back(sf::Time::Zero);
    trunkSpawnTime_.push_back(sf::Time::Zero);
    trunkSpawnTime_.push_back(sf::Time::Zero);
    
    vehicles_.resize(4);
    vehicleSpawnProgression_.resize(4);
    vehicleSpawnTime_.resize(4);
    
    for(int k = 0; k < 3; k++)
    {
        for(int i = 0; i < 4; i++)
        {
            sf::Time timing = sf::milliseconds(random(7000, 10000));
            trunks_[i].emplace_back(sf::Vector2f{(i % 2 ? -128.f : 800.f + 128.f), 40 + 28 + 56 * (3 - i)},
                                    textures_[6], (i == 0 ? -50 : i == 1 ? 60 : i == 2 ? -70 : 80), &frog_);

            for(size_t j = 0; j < trunks_[i].size(); ++j)
            {
                trunks_[i][j].update(timing);
                if(trunks_[i][j].isOut())
                {
                    trunks_[i].erase(trunks_[i].begin() + j);
                    --j;
                }

            }
        }
        for(int i = 0; i < 4; i++)
        {
            sf::Time timing = sf::milliseconds(random(2000, 5000));
            vehicles_[i].emplace_back(sf::Vector2f{(i % 2 ? -128.f : 800.f + 128.f), 40 + 28 + 56 * (8 - i)},
                                    textures_[random(1,4)==1?8:7], (i == 0 ? -80 : i == 1 ? 90 : i == 2 ? -100 : 110), &frog_);

            for(size_t j = 0; j < vehicles_[i].size(); ++j)
            {
                vehicles_[i][j].update(timing);
                if(vehicles_[i][j].isOut())
                {
                    vehicles_[i].erase(vehicles_[i].begin() + j);
                    --j;
                }

            }
        }
    }
        
        minY = frog_.position().y;
}

bool Frogger::computeFrame(const sf::Time& elapsedTime, int& score)
{
    if(lose_)
    {
        // TODO Son défaite
        return false;
    }
    
    waterAnimationProgression_ += elapsedTime;
    if(waterAnimationProgression_ >= waterAnimationTime) 
    {
        currentWaterTexture_ = (currentWaterTexture_+1) % 4;
        water_.setTexture(&textures_[currentWaterTexture_]);
        waterAnimationProgression_ = sf::Time::Zero;
    }

    for(int i = 0; i < 4; ++i)
    {
        trunkSpawnProgression_[i] += elapsedTime;
        if(trunkSpawnProgression_[i] >= trunkSpawnTime_[i])
        {
            trunks_[i].emplace_back(sf::Vector2f{(i%2?-128.f:800.f+128.f), 40+28+56*(3-i)}, textures_[6], (i==0?-50:i==1?60:i==2?-70:80), &frog_);
            trunkSpawnTime_[i] = sf::milliseconds(random(7000, 10000));
            trunkSpawnProgression_[i] = sf::Time::Zero;
        }
    }

    for(int i = 0; i < 4; ++i)
    {
        vehicleSpawnProgression_[i] += elapsedTime;
        if(vehicleSpawnProgression_[i] >= vehicleSpawnTime_[i])
        {
            vehicles_[i].emplace_back(sf::Vector2f{(i % 2 ? -128.f : 800.f + 128.f), 40 + 28 + 56 * (8 - i)},
                                      textures_[random(1,4)==1?8:7], (i == 0 ? -80 : i == 1 ? 90 : i == 2 ? -100 : 110), &frog_);
            vehicleSpawnTime_[i] = sf::milliseconds(random(2000, 5000));
            vehicleSpawnProgression_[i] = sf::Time::Zero;
        }
    }


    bool isOnTrunk = false;
    frog_.update(elapsedTime);
    for(int j = 0; j < 4; j++)
    {
        for(size_t i = 0; i < trunks_[j].size(); ++i)
        {
            trunks_[j][i].update(elapsedTime);
            isOnTrunk = isOnTrunk || trunks_[j][i].isFrogOn(frog_);
            if(trunks_[j][i].isOut())
            {
                trunks_[j].erase(trunks_[j].begin() + i);
                --i;
            }
        }
    }
    
    bool isCrushed = false;
    for(int j = 0; j < 4; j++)
    {
        for(size_t i = 0; i < vehicles_[j].size(); ++i)
        {
            vehicles_[j][i].update(elapsedTime);
            isCrushed = isCrushed || vehicles_[j][i].isFrogOn(frog_);
            if(vehicles_[j][i].isOut())
            {
                vehicles_[j].erase(vehicles_[j].begin() + i);
                --i;
            }
        }
    }
        
    if((sf::FloatRect{0, 40, 800, 56*4}.contains(frog_.position()) && !isOnTrunk) || isCrushed)
    {
        lose_ = true;
    }
    
    if(frog_.position().y < 32) {
        Dj_.play(9);
        return false;
    }
    
    if(frog_.position().y < minY)
    {
        score += 70;
    }
    
    return true;
}

void Frogger::drawState(sf::Sprite& countdown) const
{
    window_.draw(water_);
    window_.draw(ground_);
    window_.draw(countdown);
    for(auto& trunks : trunks_)
        for(auto& trunk : trunks)
            window_.draw(trunk.sprite());
    auto sprite = frogSprite_;
    sprite.setPosition(frog_.position());
    window_.draw(sprite);
    for(auto& vehicles : vehicles_)
        for(auto& vehicle : vehicles)
            window_.draw(vehicle.sprite());
}
