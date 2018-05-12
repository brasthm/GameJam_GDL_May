#include "Snake.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>


Snake::Snake(sf::RenderWindow& window) : Game{window}
{
    snake_.emplace_back(BodyPart::HEAD, sf::Vector2f{400, 280}, RIGHT);
    snake_.emplace_back(BodyPart::BODY, sf::Vector2f{360, 280}, RIGHT);
    snake_.emplace_back(BodyPart::TAIL, sf::Vector2f{320, 280}, RIGHT);
    savedOrientation_ = currentOrientation_ = RIGHT;
    
    textures_.resize(10);
    textures_[0].loadFromFile("../../img/snake/bodyH.png");
    sprites_[BodyPart::BODY][RIGHT].setTexture(textures_[0]);
    sprites_[BodyPart::BODY][LEFT].setTexture(textures_[0]);
    textures_[1].loadFromFile("../../img/snake/bodyV.png");
    sprites_[BodyPart::BODY][UP].setTexture(textures_[1]);
    sprites_[BodyPart::BODY][DOWN].setTexture(textures_[1]);
    textures_[2].loadFromFile("../../img/snake/headUp.png");
    sprites_[BodyPart::HEAD][UP].setTexture(textures_[2]);
    textures_[3].loadFromFile("../../img/snake/headLeft.png");
    sprites_[BodyPart::HEAD][LEFT].setTexture(textures_[3]);
    textures_[4].loadFromFile("../../img/snake/headDown.png");
    sprites_[BodyPart::HEAD][DOWN].setTexture(textures_[4]);
    textures_[5].loadFromFile("../../img/snake/headRight.png");
    sprites_[BodyPart::HEAD][RIGHT].setTexture(textures_[5]);
    textures_[6].loadFromFile("../../img/snake/tailUp.png");
    sprites_[BodyPart::TAIL][UP].setTexture(textures_[6]);
    textures_[7].loadFromFile("../../img/snake/tailLeft.png");
    sprites_[BodyPart::TAIL][LEFT].setTexture(textures_[7]);
    textures_[8].loadFromFile("../../img/snake/tailDown.png");
    sprites_[BodyPart::TAIL][DOWN].setTexture(textures_[8]);
    textures_[9].loadFromFile("../../img/snake/tailRight.png");
    sprites_[BodyPart::TAIL][RIGHT].setTexture(textures_[9]);
    
}


void Snake::computeFrame(const sf::Time& elapsedTime)
{
    cycleProgression_ += elapsedTime;

    for(auto& part : snake_)
        part.update(elapsedTime);
    
    if(cycleProgression_ >= cycleTime)
    {
        cycleProgression_ = sf::Time::Zero;
        savedOrientation_ = currentOrientation_;
        for(auto it = snake_.rbegin(); it+1 != snake_.rend(); ++it)
        {
            it->move((it+1)->orientation());
            it->orientation((it+1)->orientation());
        }
        snake_.front().move(savedOrientation_);
        snake_.front().orientation(savedOrientation_);
        
        auto nextPos = snake_.front().nextPosition();
        if(nextPos.x < 0 or nextPos.y < 0 or nextPos.x >= 800 or nextPos.y >= 600)
            lose = true;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) and savedOrientation_ != DOWN and savedOrientation_ != UP)
        currentOrientation_ = UP;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) and savedOrientation_ != RIGHT and savedOrientation_ != LEFT)
        currentOrientation_ = LEFT;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) and savedOrientation_ != UP and savedOrientation_ != DOWN)
        currentOrientation_ = DOWN;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) and savedOrientation_ != LEFT and savedOrientation_ != RIGHT)
        currentOrientation_ = RIGHT;
}

void Snake::drawState() const
{
    auto size = window_.getSize();
    sf::RectangleShape rect(window_.mapPixelToCoords({size.x, size.y}));
    rect.setFillColor(lose ? sf::Color::Red : sf::Color::Blue);
    window_.draw(rect);
    
    if(!lose)
    {
        for(auto it = snake_.rbegin(); it != snake_.rend(); ++it)
        {
            auto sprite = sprites_[it->type()][it->orientation()];
            sprite.setPosition(it->position());
            window_.draw(sprite);
        }
    }
}
