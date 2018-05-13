#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "BananaHero.hpp"
#include "../../utilities.hpp"


BananaHero::BananaHero(sf::RenderTarget& window) : Game(window)
{
    textures_.resize(7);
    textures_[0].loadFromFile("../../img/bananaHero/banana.png");
    textures_[1].loadFromFile("../../img/bananaHero/doublebanana.png");
    textures_[2].loadFromFile("../../img/bananaHero/rottenbanana.png");
    textures_[3].loadFromFile("../../img/bananaHero/rottendoublebanana.png");
    textures_[4].loadFromFile("../../img/bananaHero/monkey.png");
    textures_[5].loadFromFile("../../img/bananaHero/sadmonkey.png");
    textures_[6].loadFromFile("../../img/bananaHero/background.png");
    monkey_.setTextures(textures_[4], textures_[5]);
    bg_.setTexture(textures_[6]);
    
    spawnDelay_ = sf::milliseconds(random(100, 400));

    x = 400;
    
    sf::Time generatedTime;
    
    while(generatedTime < sf::milliseconds(400))
    {
        generateBanana();
        auto timing = sf::milliseconds(random(100, 400));
        for(auto& banana : bananas_)
            banana.update(timing);
        generatedTime += timing;
    }
    generateBanana();
    
    monkey_.position({400, 500});
}

bool BananaHero::computeFrame(const sf::Time& elapsedTime, int& score)
{
    spawnProgression_ += elapsedTime;
    if(spawnProgression_ >= spawnDelay_) {
        generateBanana();
        spawnProgression_ = sf::Time::Zero;
        spawnDelay_ = sf::milliseconds(random(100, 400));
    }
    
    float nextX = monkey_.position().x;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        nextX -= monkeySpeed*elapsedTime.asSeconds();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        nextX += monkeySpeed*elapsedTime.asSeconds();
    nextX = std::min(std::max(nextX, 80.f), 720.f);
    monkey_.position({nextX, monkey_.position().y});
    
    for(size_t i = 0; i < bananas_.size(); ++i)
    {
        bananas_[i].update(elapsedTime);
        
        if(monkey_.receive(bananas_[i])) {
            switch(bananas_[i].type())
            {
                case Banana::NORMAL:
                    monkey_.happy(true);
                    score += 100;// TODO Banane normale attrappée : des points en plus
                    break;
                
                case Banana::DOUBLE:
                    monkey_.happy(true);
                    score += 300;// TODO Banane double attrappée : plus de points en plus
                    break;
                
                case Banana::ROTTEN:
                    monkey_.happy(false);
                    score -= 100;// TODO Banane pourrie attrappée : des points en moins
                    break;
                
                case Banana::DOUBLEROTTEN:
                    monkey_.happy(false);
                    score -= 300;// TODO Banane double pourrie attrappée : plus de points en moins
                    break;
            }
            bananas_.erase(bananas_.begin()+i);
            --i;
        } 
        else if(bananas_[i].position().y > 640)
        {
            if(bananas_[i].type() == Banana::NORMAL || bananas_[i].type() == Banana::DOUBLE)
                score -= 50; // TODO Banane normale ou double pas attrapée : des points en moins
            bananas_.erase(bananas_.begin()+i);
            --i;
        }
    }
        
    return true;
}

void BananaHero::drawState(sf::Sprite &countdown) const
{
    window_.draw(bg_);
	window_.draw(countdown);
    
    for(auto& banana : bananas_)
    {
        sf::Sprite sprite;
        sprite.setTexture(textures_[banana.type()]);
        sprite.setOrigin(40, 40);
        sprite.setPosition(banana.position());
        window_.draw(sprite);
    }
    
    window_.draw(monkey_.sprite());
}

void BananaHero::generateBanana()
{
    Banana::type_t type;
    if(random(1, 10) == 1) {
        if(random(1,5) == 1)
            type = Banana::DOUBLEROTTEN;
        else
            type = Banana::DOUBLE;
    }else {
        if(random(1,5) == 1)
            type = Banana::ROTTEN;
        else
            type = Banana::NORMAL;
    }
    if(random(1, 50) == 1)
        x += random(-500, 500);
    x = std::clamp(x + random(-100, 100), 40.f, 760.f);
    bananas_.emplace_back(type, x);
}
