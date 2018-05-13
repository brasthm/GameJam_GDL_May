#include "DirectionalSprite.h"

DirectionalSprite::DirectionalSprite()
{
	for (size_t i = 0; i < 4; i++)
		sprites_.emplace_back();

	current_ = RIGHT;
}

void DirectionalSprite::setTexture(orientation_t dir, std::string path)
{
	sprites_[dir].setTexture(path);
}

void DirectionalSprite::move(float deltaX, float deltaY)
{
	sprites_[current_].move(deltaX, deltaY);
}


void DirectionalSprite::setPosition(float deltaX, float deltaY)
{
	sprites_[current_].setPosition(deltaX, deltaY);
}

void DirectionalSprite::setDelay(int delay)
{
	for (size_t i = 0; i < sprites_.size(); i++)
		sprites_[i].setDelay(delay);
}

void DirectionalSprite::setDelay(int delay, int moveDelay, int moveAnimDelay)
{
	for (size_t i = 0; i < sprites_.size(); i++)
		sprites_[i].setDelay(delay, moveDelay, moveAnimDelay);
}

void DirectionalSprite::setTileSize(size_t w, size_t h)
{
	for (size_t i = 0; i < sprites_.size(); i++)
		sprites_[i].setTileSize(w, h);
}

void DirectionalSprite::updateMap(Map * m)
{
	for (size_t i = 0; i < sprites_.size(); i++)
	{
		sprites_[i].updateMap(m);
	}
}

void DirectionalSprite::changeDirection(orientation_t dir)
{
	sf::Vector2f pos = sprites_[current_].getPosition();
	sf::Vector2f prevPos = sprites_[current_].getPrevPosition();
	sf::Vector2f nextPos = sprites_[current_].getNextPosition();
	sf::Time clock = sprites_[current_].getClock();

	orientation_t direction = sprites_[current_].getDirection();

	for (size_t i = 0; i < sprites_.size(); i++)
	{
		sprites_[i].setClcok(clock);
		sprites_[i].setPos(pos.x, pos.y);
		sprites_[i].setDirection(direction, dir);
		sprites_[i].setNextPosition(nextPos.x, nextPos.y);
		sprites_[i].setPrevPosition(prevPos.x, prevPos.y);
	}
}

void DirectionalSprite::update(sf::Time elapsed)
{
	current_ = sprites_[current_].update(elapsed);

	sf::Vector2f pos = sprites_[current_].getPosition();
	sf::Vector2f prevPos = sprites_[current_].getPrevPosition();
	sf::Vector2f nextPos = sprites_[current_].getNextPosition();
	sf::Time clock = sprites_[current_].getClock();

	orientation_t dir = sprites_[current_].getDirection();
	orientation_t nDir = sprites_[current_].getNextDirection();
	

	for (size_t i = 0; i < sprites_.size(); i++)
	{
		sprites_[i].setDirection(dir, nDir);
		if(i != current_)
			sprites_[i].updateClock(elapsed);
		sprites_[i].setClcok(clock);
		sprites_[i].setPos(pos.x, pos.y);
		sprites_[i].setNextPosition(nextPos.x, nextPos.y);
		sprites_[i].setPrevPosition(prevPos.x, prevPos.y);
	}
}

