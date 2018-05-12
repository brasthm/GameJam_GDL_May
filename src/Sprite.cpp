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

	sprite_.setPosition(position_);
}

void Sprite::move(Map & map, float deltaX, float deltaY)
{
	sf::FloatRect box = sprite_.getGlobalBounds();
	sf::Vector2f newPosition = { position_.x + deltaX, position_.y + deltaY };


	if (deltaX > 0 && !map.isBlank(position_.x + box.width + deltaX, position_.y) && !map.isBlank(position_.x + box.width + deltaX, position_.y + box.height))
	{
		sf::Vector2i pos = map.getTile(position_.x + box.width + deltaX, position_.y);
		newPosition = map.getPosition(pos.x - 1, pos.y);
		newPosition.y = position_.y;
	}

	if (deltaX < 0 && !map.isBlank(position_.x + deltaX, position_.y) && !map.isBlank(position_.x + deltaX, position_.y))
	{
		sf::Vector2i pos = map.getTile(position_.x + deltaX, position_.y);
		newPosition = map.getPosition(pos.x + 1, pos.y);
		newPosition.y = position_.y;
	}
	
	if (deltaY > 0 && !map.isBlank(position_.x, position_.y + box.height + deltaY) && !map.isBlank(position_.x + box.width, position_.y + box.height + deltaY))
	{
		sf::Vector2i pos = map.getTile(position_.x, position_.y + box.height + deltaY);
		newPosition = map.getPosition(pos.x, pos.y - 1);
		newPosition.x = position_.x;
	}

	if (deltaY < 0 && !map.isBlank(position_.x, position_.y + deltaY) && !map.isBlank(position_.x + box.width, position_.y + deltaY))
	{
		sf::Vector2i pos = map.getTile(position_.x, position_.y + deltaY);
		newPosition = map.getPosition(pos.x, pos.y + 1);
		newPosition.x = position_.x;
	}

	position_ = newPosition;
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

const bool Sprite::isOOB()
{
	sf::FloatRect box = sprite_.getGlobalBounds();
	return position_.x + box.width < 0 || position_.x > 800 || position_.y + box.height < 0 || position_.y > 600;
}
