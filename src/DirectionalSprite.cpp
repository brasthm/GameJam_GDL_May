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

void DirectionalSprite::move(Map & map, float deltaX, float deltaY)
{
	sprites_[current_].move(map, deltaX, deltaY);
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

void DirectionalSprite::changeDirection(orientation_t dir)
{
	sf::Vector2f pos = sprites_[current_].getPosition();
	current_ = dir;
	sprites_[current_].setPosition(pos.x, pos.y);
}

void DirectionalSprite::update()
{
	sprites_[current_].update();
}
