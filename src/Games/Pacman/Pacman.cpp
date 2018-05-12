#include "Pacman.hpp"

void Pacman::fillGum()
{
	
	for (size_t i = 0; i < 800 / map_.getTileWidth(); i++)
	{
		for (size_t j = 0; j < 600 / map_.getTileHeight(); j++)
		{
			if (map_.getValue(i, j) == 0)
			{
				gum_.emplace_back();
				gum_.back().setTexture("../../img/pacman/gum.png");
				gum_.back().setPos(i * map_.getTileWidth(), j * map_.getTileHeight());
			}
		}
	}

	for(size_t i = 0; i < gum_.size(); i++)
		gum_[i].applyTexture();
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


	fillGum();
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

	pacman_.update();

	if (pacman_.isOOB())
	{
		orientation_t dir = pacman_.getDirection();
		if (dir == UP) pacman_.setPosition(pacman_.getPosition().x, 0);
		if (dir == DOWN) pacman_.setPosition(pacman_.getPosition().x, 600 - map_.getTileHeight());
		if (dir == LEFT) pacman_.setPosition(800 - map_.getTileWidth(), pacman_.getPosition().y);
		if (dir == RIGHT) pacman_.setPosition(0, pacman_.getPosition().y);
	}
}

void Pacman::drawState() const 
{
	map_.draw(window_);
	
	for (size_t i = 0; i < gum_.size(); i++)
		window_.draw(gum_[i].getSprite());

	window_.draw(pacman_.getSprite());
}

