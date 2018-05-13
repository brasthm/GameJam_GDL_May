#include "Snake.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <random>
#include <iostream>


Snake::Snake(sf::RenderTarget& window) : Game{window}
{
    snake_.emplace_back(BodyPart::HEAD, sf::Vector2f{400, 280}, RIGHT);
    snake_.emplace_back(BodyPart::BODY, sf::Vector2f{360, 280}, RIGHT);
    snake_.emplace_back(BodyPart::TAIL, sf::Vector2f{320, 280}, RIGHT);
    
    textures_.resize(12);
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
    
    textures_[10].loadFromFile("../../img/snake/apple.png");
    appleSprite_.setTexture(textures_[10]);
    
    textures_[11].loadFromFile("../../img/snake/background.png");
    bg_.setTexture(textures_[11]);
    
    randomSnake(3, 50);
    savedOrientation_ = currentOrientation_ = snake_.front().orientation();

    randomApple();
}


bool Snake::computeFrame(const sf::Time& elapsedTime, int& score)
{
    if(lose)
        return false;
    
    cycleProgression_ += elapsedTime;

    for(auto& part : snake_)
        part.update(elapsedTime);
    
    if(cycleProgression_ >= cycleTime)
    {
        cycleProgression_ = sf::Time::Zero;
        savedOrientation_ = currentOrientation_;
        
        if(appleAte)
        {
            auto rit = snake_.rbegin()+1;
            snake_.insert(rit.base(), *rit);
            randomApple();
            score += 20 * snake_.size(); // TODO Pomme mangée, augmentation de score
        }
        for(auto it = snake_.rbegin() + (appleAte?2:0); it+1 != snake_.rend(); ++it)
        {
            it->move((it+1)->orientation());
            it->orientation((it+1)->orientation());
        }
        appleAte = false;
        snake_.front().move(savedOrientation_);
        snake_.front().orientation(savedOrientation_);
        
        auto nextPos = snake_.front().nextPosition();
        if(nextPos.x < 0 || nextPos.y < 0 || nextPos.x >= 800 || nextPos.y >= 600)
            lose = true;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && savedOrientation_ != DOWN && savedOrientation_ != UP)
        currentOrientation_ = UP;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && savedOrientation_ != RIGHT && savedOrientation_ != LEFT)
        currentOrientation_ = LEFT;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && savedOrientation_ != UP && savedOrientation_ != DOWN)
        currentOrientation_ = DOWN;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && savedOrientation_ != LEFT && savedOrientation_ != RIGHT)
        currentOrientation_ = RIGHT;
    
    if(applePos_ == snake_.front().nextPosition())
        appleAte = true;
    
    for(auto it = snake_.begin()+1; it != snake_.end(); ++it)
        if(snake_.front().nextPosition() == it->nextPosition())
            lose = true;
    
    return true;
}

void Snake::drawState(sf::Sprite &countdown) const
{
    window_.draw(bg_);
	window_.draw(countdown);
    
    auto appleSprite = appleSprite_;
    appleSprite.setPosition(applePos_);
    window_.draw(appleSprite);
    
    for(auto it = snake_.rbegin(); it != snake_.rend(); ++it)
    {
        auto sprite = sprites_[it->type()][it->orientation()];
        sprite.setPosition(it->position());
        window_.draw(sprite);
    }
}

void Snake::randomApple()
{
    static std::random_device rd;
    static std::default_random_engine gen(rd());
    std::uniform_int_distribution<int> xDistrib(0, 19);
    std::uniform_int_distribution<int> yDistrib(0, 14);
    bool valid;
    do
    {
        applePos_.x = 40 * xDistrib(gen);
        applePos_.y = 40 * yDistrib(gen);
        valid = true;
        
        for(auto& part : snake_)
            if(applePos_ == part.nextPosition())
                valid = false;
        if(applePos_ == snake_.back().lastPosition())
            valid = false;
    } while(!valid);
}

void Snake::randomSnake(size_t bodyPartMin, size_t bodyPartMax)
{
    static std::random_device rd;
    static std::default_random_engine gen(rd());
    
    std::uniform_int_distribution<int> xDistrib(0, 19);
    std::uniform_int_distribution<int> yDistrib(0, 14);
    std::uniform_int_distribution<int> uniformDir(0, 3);
    std::uniform_int_distribution<int> biasedDir(0,1);
    std::uniform_int_distribution<size_t> bodyParts(bodyPartMin, bodyPartMax);
    
    bool valid;

    do
    {
        valid = true;
        
        snake_.clear();
        snake_.emplace_back(BodyPart::HEAD, sf::Vector2f{xDistrib(gen)*40.f, yDistrib(gen)*40.f}, orientation_t(uniformDir(gen)));
        
        size_t partNumber = bodyParts(gen);
        size_t step = 0;
        while(snake_.size() <= partNumber+1 && valid)
        {            
            auto ori = snake_.back().orientation();
            orientation_t next_ori = orientation_t((ori + biasedDir(gen)+biasedDir(gen)-1 + 4) % 4);
            auto pos = snake_.back().position() + sf::Vector2f{(ori%2) * (ori-2) * 40.f, ((ori+1)%2) * (1-ori) * 40.f};
            
            bool overlap = false;
            for(auto& part : snake_)
                if(part.position() == pos)
                {
                    overlap = true;
                    break;
                }
            
            if(pos.x < 0 || pos.y < 0 || pos.x >= 800 || pos.y >= 600 || overlap)
            {
                snake_.pop_back();
                if(snake_.empty())
                    valid = false;
            }
            else
            {
                snake_.emplace_back(BodyPart::BODY, pos, next_ori);
            }
            
            if(++step > 5*partNumber)
                valid = false;
            
        }
        for(int i = 1; i < 8 && valid; i++)
        {
            auto ori = snake_.front().orientation();
            auto pos = snake_.front().position() + sf::Vector2f{(ori%2) * (2-ori) * 40.f, ((ori+1)%2) * (ori-1) * 40.f} * (float)i;
            bool overlap = false;
            for(auto& part : snake_)
                if(part.position() == pos)
                {
                    overlap = true;
                    break;
                }
            if(pos.x < 0 || pos.y < 0 || pos.x >= 800 || pos.y >= 600 || overlap)
                valid = false;
        }
    } while(!valid);
    
    snake_.back().type(BodyPart::TAIL);
}

