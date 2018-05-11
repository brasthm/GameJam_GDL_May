#include "Sprite.h"

Sprite::Sprite()
{
	clock_.restart();
}

void Sprite::setTexture(std::string path)
{
	textures_.emplace_back();
	if (!textures_.back().loadFromFile(path))
		std::cerr << "Loading failed : " << path << " doesn't exist" << std::endl;
	else
		nb_textures_++;
}

void Sprite::move(float deltaX, float deltaY)
{
	position_.x += deltaX;
	position_.y += deltaY;

	std::cerr << deltaX << " " << deltaY << std::endl;

	sprite_.setPosition(position_);
}

void Sprite::setPosition(float deltaX, float deltaY)
{
	position_.x = deltaX;
	position_.y = deltaY;

	sprite_.setPosition(position_);
}

void Sprite::setDelay(int delay)
{
	delay_ = sf::milliseconds(delay);
}

void Sprite::update()
{
	sf::Time elapsed = clock_.getElapsedTime();

	if (elapsed >= delay_)
	{
		current_++;
		if (current_ == nb_textures_)
			current_ = 0;

		sprite_.setTexture(textures_[current_]);
		clock_.restart();
	}
}
