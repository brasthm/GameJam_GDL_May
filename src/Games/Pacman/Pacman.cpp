#include "Pacman.hpp"

void Pacman::fillGum()
{
	
	for (size_t i = 0; i < 800 / map_.getTileWidth(); i++)
	{
		for (size_t j = 0; j < 600 / map_.getTileHeight(); j++)
		{
			if (map_.getValue(i, j) == 0)
			{
				if (!(j == 5 && i == 0) && !(j == 5 && i == 1) && !(j == 9 && i == 0) && !(j == 9 && i == 1) && 
					!(j == 5 && i == 18) && !(j == 5 && i == 19) && !(j == 9 && i == 18) && !(j == 9 && i == 19))
					gums_.emplace_back(i, j);
			}
		}
	}
}

void Pacman::collision()
{
	for (size_t i = 0; i < gums_.size(); i++)
	{
		if (map_.getTile(pacman_.getPosition().x + 20, pacman_.getPosition().y + 20) == gums_[i])
		{
			gums_.erase(gums_.begin() + i);
			i--;
		}
	}

	if (fruitAlive_ && map_.getTile(pacman_.getPosition().x + 20, pacman_.getPosition().y + 20) == (map_.getTile(fruit_.getPosition().x + 20, fruit_.getPosition().y + 20)))
		fruitAlive_ = false;
}

void Pacman::ia()
{
	static std::random_device rd;
	static std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> uniform(0, 3);


	if (fantomeRouge_.isIAReady())
		fantomeRouge_.changeDirection((orientation_t)uniform(rd));
	
	
}

orientation_t Pacman::getDirection(sf::Vector2i source, sf::Vector2i destination)
{
	sf::Vector2i delta = destination - source;

	if (delta.x > 0) return RIGHT;
	if (delta.x < 0) return LEFT;
	if (delta.y > 0) return DOWN;
	if (delta.x < 0) return UP;
}

Pacman::Pacman(sf::RenderWindow & window) : Game{ window }
{
	map_.setTileSize(40, 40);

	map_.addTexture("blank");
	map_.addTexture("../../img/pacman/wall_h.png");
	map_.addTexture("../../img/pacman/wall_v.png");
	map_.addTexture("../../img/pacman/wall_corner_topleft.png");
	map_.addTexture("../../img/pacman/wall_corner_topright.png");
	map_.addTexture("../../img/pacman/wall_corner_bottomleft.png");
	map_.addTexture("../../img/pacman/wall_corner_bottomright.png");
	map_.addTexture("../../img/pacman/wall_end_left.png");
	map_.addTexture("../../img/pacman/wall_end_right.png");
	map_.addTexture("../../img/pacman/wall_end_up.png");
	map_.addTexture("../../img/pacman/wall_end_down.png");
	map_.addTexture("../../img/pacman/wall_three_left.png");
	map_.addTexture("../../img/pacman/wall_three_right.png");
	map_.addTexture("../../img/pacman/wall_three_top.png");
	map_.addTexture("../../img/pacman/wall_three_bottom.png");
	map_.addTexture("../../img/pacman/wall_block.png");

	map_.loadFromFile("../../maps/pacman.txt");

	pacman_.setTexture(RIGHT, "../../img/pacman/pacman1_right.png");
	pacman_.setTexture(RIGHT, "../../img/pacman/pacman2_right.png");
	pacman_.setTexture(LEFT, "../../img/pacman/pacman1_left.png");
	pacman_.setTexture(LEFT, "../../img/pacman/pacman2_left.png");
	pacman_.setTexture(UP, "../../img/pacman/pacman1_up.png");
	pacman_.setTexture(UP, "../../img/pacman/pacman2_up.png");
	pacman_.setTexture(DOWN, "../../img/pacman/pacman1_down.png");
	pacman_.setTexture(DOWN, "../../img/pacman/pacman2_down.png");

	pacman_.updateMap(&map_);
	pacman_.setDelay(200, 200, 200);
	pacman_.setTileSize(40, 40);
	pacman_.setPosition(40, 40);

	gum_.loadFromFile("../../img/pacman/gum.png");
	fillGum();

	for (size_t i = 0; i < gums_.size(); i++)
		noeuds_.push_back(gums_[i]);

	static std::random_device rd;
	static std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> uniform(0, gums_.size() - 1), uniform2(0, 2);

	switch (uniform2(gen))
	{
		case 0:
			fruit_.setTexture("../../img/pacman/apple.png");
			break;
		case 1:
			fruit_.setTexture("../../img/pacman/cherry.png");
			break;
		case 2:
			fruit_.setTexture("../../img/pacman/orange.png");
			break;
	}

	fruit_.applyTexture();

	int n = uniform(gen);

	fruit_.setPos(gums_[n].x * map_.getTileWidth(), gums_[n].y* map_.getTileHeight());
	gums_.erase(gums_.begin() + n);

	fantomeRouge_.setTexture("../../img/pacman/red_ghost.png");
	fantomeRouge_.updateMap(&map_);
	fantomeRouge_.setDelay(200, 200, 200);
	fantomeRouge_.setTileSize(40, 40);
	fantomeRouge_.setPosition(40, 40 * 3);
	fantomeRouge_.applyTexture();
	
}

void Pacman::computeFrame(const sf::Time & elapsedTime)
{
	if (keyboard_.isKeyPressed(sf::Keyboard::Up))
	{
		pacman_.changeDirection(UP);
	}
	else if (keyboard_.isKeyPressed(sf::Keyboard::Down))
	{
		pacman_.changeDirection(DOWN);
	}
	else if (keyboard_.isKeyPressed(sf::Keyboard::Left))
	{
		pacman_.changeDirection(LEFT);
	}
	else if (keyboard_.isKeyPressed(sf::Keyboard::Right))
	{
		pacman_.changeDirection(RIGHT);
	}

	ia();

	pacman_.update();
	fantomeRouge_.update();

	if (pacman_.isOOB())
	{
		orientation_t dir = pacman_.getDirection();
		if (dir == UP) pacman_.setPosition(pacman_.getPosition().x, 0);
		if (dir == DOWN) pacman_.setPosition(pacman_.getPosition().x, 600 - map_.getTileHeight());
		if (dir == LEFT) pacman_.setPosition(800 - map_.getTileWidth(), pacman_.getPosition().y);
		if (dir == RIGHT) pacman_.setPosition(0, pacman_.getPosition().y);
	}

	collision();
}

void Pacman::drawState() const 
{
	map_.draw(window_);

	for (size_t i = 0; i < gums_.size(); i++)
	{
		sf::Sprite temp;

		temp.setTexture(gum_);
		temp.setPosition(gums_[i].x * map_.getTileWidth(), gums_[i].y * map_.getTileHeight());
		window_.draw(temp);
	}

	if(fruitAlive_)
		window_.draw(fruit_.getSprite());

	window_.draw(fantomeRouge_.getSprite());
	window_.draw(pacman_.getSprite());
}

