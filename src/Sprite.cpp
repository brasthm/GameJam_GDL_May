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

	if (auto_ && map_ != nullptr)
	{
		if (!map_->isBlank(nextPosition_.x + 20, nextPosition_.y + 20))
			nextPosition_ = prevPosition_;
	}

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

void Sprite::setClcok(sf::Time c)
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

void Sprite::setDirection(orientation_t dir, orientation_t nDir)
{
	direction_ = dir;
	nextDirection_ = nDir;
}

void Sprite::applyTexture()
{
	sprite_.setTexture(textures_.back());
}

void Sprite::applyTexture(size_t n)
{
	if(n <nb_textures_) sprite_.setTexture(textures_[n]);
}

void Sprite::updateClock(sf::Time elapsed)
{
	clock_ += elapsed;
	clockMoveAnim_ += elapsed;
	clockMove_ += elapsed;
}

orientation_t Sprite::update(sf::Time elapsed)
{
	clock_ +=elapsed;

	if (clock_ >= delay_)
	{
		current_++;
		if (current_ == nb_textures_)
			current_ = 0;

		sprite_.setTexture(textures_[current_]);
		clock_ = sf::milliseconds(0);
	}

	if (auto_)
	{
		iaReady_ = false;

		clockMoveAnim_ += elapsed;
		float ratio = clockMoveAnim_.asSeconds() / moveAnimDelay_.asSeconds();
		if (ratio < 1)
		{
			sf::Vector2f pos = prevPosition_ + (nextPosition_ - prevPosition_) * ratio;
			position_ = pos;
			sprite_.setPosition(position_);
		}
	
		clockMove_ += elapsed;
		
		if (clockMove_ > moveDelay_ || nextPosition_ == prevPosition_)
		{
			if (direction_ == nextDirection_)
			{
				prevPosition_ = nextPosition_;
				setNextPosition();

				if (!map_->isBlank(nextPosition_.x + 20, nextPosition_.y + 20))
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

				if (!map_->isBlank(nextPosition_.x + 20, nextPosition_.y + 20))
				{
					direction_ = temp;
					nextPosition_ = prevPosition_;
					setNextPosition();
					if (!map_->isBlank(nextPosition_.x + 20, nextPosition_.y + 20))
						nextPosition_ = prevPosition_;
				}
			}

			clockMove_ = sf::milliseconds(0);
			clockMoveAnim_ = sf::milliseconds(0);
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
