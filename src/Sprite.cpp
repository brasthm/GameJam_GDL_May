#include "Sprite.h"

void Sprite::setNextPosition()
{
	if (direction_ == UP)
		nextPosition_.y -= tileHeight_;
	else if (direction_ == DOWN)
		nextPosition_.y += tileHeight_;
	else if (direction_ == RIGHT)
		nextPosition_.x += tileWidth_;
	else if (direction_ == LEFT)
		nextPosition_.x -= tileWidth_;
}

Sprite::Sprite() 
{
	direction_ = nextDirection_ = RIGHT;
	clock_.restart();
	clockMove_.restart();
	clockMoveAnim_.restart();

	nextPosition_ = { 40, 40 };
	prevPosition_ = { 40, 40 };

	auto_ = true;
}

Sprite::~Sprite()
{

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

	prevPosition_ = position_;
	nextPosition_ = position_;
	setNextPosition();

	sprite_.setPosition(position_);
}

void Sprite::setPosition(float deltaX, float deltaY)
{
	position_.x = deltaX;
	position_.y = deltaY;

	prevPosition_ = position_;
	nextPosition_ = position_;
	setNextPosition();

	sprite_.setPosition(position_);
}

void Sprite::setPos(float deltaX, float deltaY)
{
	position_.x = deltaX;
	position_.y = deltaY;

	sprite_.setPosition(position_);
}

void Sprite::setPrevPosition(float deltaX, float deltaY)
{
	prevPosition_.x = deltaX;
	prevPosition_.y = deltaY;
}

void Sprite::setNextPosition(float deltaX, float deltaY)
{
	nextPosition_.x = deltaX;
	nextPosition_.y = deltaY;
}

void Sprite::setClcok(sf::Clock c)
{
	clockMoveAnim_ = c;
}

void Sprite::setDelay(int delay)
{
	delay_ = sf::milliseconds(delay);
}

void Sprite::setDelay(int delay, int moveDelay, int moveAnimDelay)
{
	delay_ = sf::milliseconds(delay);
	moveDelay_ = sf::milliseconds(moveDelay);
	moveAnimDelay_ = sf::milliseconds(moveAnimDelay);
}

void Sprite::setTileSize(size_t w, size_t h)
{
	tileWidth_ = w;
	tileHeight_ = h;

	setNextPosition();
}

void Sprite::changeDirection(orientation_t dir)
{
	nextDirection_ = dir;
}

void Sprite::applyTexture()
{
	sprite_.setTexture(textures_.back());
}

orientation_t Sprite::update()
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

	if (auto_)
	{
		iaReady_ = false;

		elapsed = clockMoveAnim_.getElapsedTime();
		float ratio = elapsed.asSeconds() / moveAnimDelay_.asSeconds();
		if (ratio < 1)
		{
			sf::Vector2f pos = prevPosition_ + (nextPosition_ - prevPosition_) * ratio;
			position_ = pos;
			sprite_.setPosition(position_);
		}
	
		elapsed = clockMove_.getElapsedTime();
		
		if (elapsed > moveDelay_)
		{
			if (direction_ == nextDirection_)
			{
				prevPosition_ = nextPosition_;
				setNextPosition();

				if (!map_->isBlank(nextPosition_.x, nextPosition_.y))
				{
					iaReady_ = true;
					nextPosition_ = prevPosition_;
				}
					
			}
			else
			{
				orientation_t temp = direction_;
				direction_ = nextDirection_;
				prevPosition_ = nextPosition_;
				setNextPosition();
				iaReady_ = true;

				if (!map_->isBlank(nextPosition_.x, nextPosition_.y))
				{
					direction_ = temp;
					nextPosition_ = prevPosition_;
					setNextPosition();
					if (!map_->isBlank(nextPosition_.x, nextPosition_.y))
						nextPosition_ = prevPosition_;
				}
			}

			clockMove_.restart();
			clockMoveAnim_.restart();
		}
	}

	return direction_;
}

sf::Vector2i Sprite::getBackTile(float x, float y)
{
	sf::Vector2i pos = map_->getTile(x, y);

	switch (direction_)
	{
		case UP:
			pos.y++;
			break;
		case DOWN:
			pos.y--;
			break;
		case RIGHT:
			pos.x--;
			break;
		case LEFT:
			pos.x++;
			break;
	}

	return pos;
}

const bool Sprite::isOOB()
{
	sf::FloatRect box = sprite_.getGlobalBounds();
	return position_.x + box.width < 0 || position_.x > 800 || position_.y + box.height < 0 || position_.y > 600;
}
