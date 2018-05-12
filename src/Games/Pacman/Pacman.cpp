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

	for (size_t i = 0; i < ghosts_.size(); i++)
	{
		if (map_.getTile(pacman_.getPosition().x + 20, pacman_.getPosition().y + 20) == (map_.getTile(ghosts_[i].getPosition().x + 20, ghosts_[i].getPosition().y + 20)))
		{
			std::cerr << "dead" << std::endl;
		}
	}

}

void Pacman::ia()
{
	static std::random_device rd;
	static std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> uniform(0, 3);

	for (size_t i = 0; i < ghosts_.size(); i++)
	{
		if (ghosts_[i].isIAReady())
		{
			orientation_t dir;
			bool redraw = false;

			do
			{
				redraw = false;
				dir = (orientation_t)uniform(rd);
				if (ghosts_[i].getDirection() == UP && dir == DOWN) redraw = true;
				if (ghosts_[i].getDirection() == DOWN && dir == UP) redraw = true;
				if (ghosts_[i].getDirection() == LEFT && dir == RIGHT) redraw = true;
				if (ghosts_[i].getDirection() == RIGHT && dir == LEFT) redraw = true;

			} while (redraw);

			ghosts_[i].changeDirection(dir);
		}
	}
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

	static std::random_device rd;
	static std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> uniform(0, gums_.size() - 1), uniform2(0, 2);
	int n;

	n = uniform(gen);
	pacman_.setPosition(40 * gums_[n].x, 40 * gums_[n].y);

	n = uniform(gen);
	ghosts_.emplace_back();
	ghosts_.back().setTexture("../../img/pacman/red_ghost.png");
	ghosts_.back().updateMap(&map_);
	ghosts_.back().setDelay(200, 200, 200);
	ghosts_.back().setTileSize(40, 40);
	ghosts_.back().setPosition(40 * gums_[n].x, 40 * gums_[n].y);
	ghosts_.back().applyTexture();

	n = uniform(gen);
	ghosts_.emplace_back();
	ghosts_.back().setTexture("../../img/pacman/orange_ghost.png");
	ghosts_.back().updateMap(&map_);
	ghosts_.back().setDelay(200, 200, 200);
	ghosts_.back().setTileSize(40, 40);
	ghosts_.back().setPosition(40 * gums_[n].x, 40 * gums_[n].y);
	ghosts_.back().applyTexture();

	n = uniform(gen);
	ghosts_.emplace_back();
	ghosts_.back().setTexture("../../img/pacman/blue_ghost.png");
	ghosts_.back().updateMap(&map_);
	ghosts_.back().setDelay(200, 200, 200);
	ghosts_.back().setTileSize(40, 40);
	ghosts_.back().setPosition(40 * gums_[n].x, 40 * gums_[n].y);
	ghosts_.back().applyTexture();

	n = uniform(gen);
	ghosts_.emplace_back();
	ghosts_.back().setTexture("../../img/pacman/pink_ghost.png");
	ghosts_.back().updateMap(&map_);
	ghosts_.back().setDelay(200, 200, 200);
	ghosts_.back().setTileSize(40, 40);
	ghosts_.back().setPosition(40 * gums_[n].x, 40 * gums_[n].y);
	ghosts_.back().applyTexture();


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

	n = uniform(gen);

	fruit_.setPos(gums_[n].x * map_.getTileWidth(), gums_[n].y* map_.getTileHeight());
	gums_.erase(gums_.begin() + n);	
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

	for (size_t i = 0; i < ghosts_.size(); i++)
		ghosts_[i].update();

	if (pacman_.isOOB())
	{
		orientation_t dir = pacman_.getDirection();
		if (dir == UP) pacman_.setPosition(pacman_.getPosition().x, 0);
		if (dir == DOWN) pacman_.setPosition(pacman_.getPosition().x, 600 - map_.getTileHeight());
		if (dir == LEFT) pacman_.setPosition(800 - map_.getTileWidth(), pacman_.getPosition().y);
		if (dir == RIGHT) pacman_.setPosition(0, pacman_.getPosition().y);
	}

	for (size_t i = 0; i < ghosts_.size(); i++)
	{
		if (ghosts_[i].isOOB())
		{
			orientation_t dir = ghosts_[i].getDirection();
			if (dir == UP) ghosts_[i].setPosition(ghosts_[i].getPosition().x, 0);
			if (dir == DOWN) ghosts_[i].setPosition(ghosts_[i].getPosition().x, 600 - map_.getTileHeight());
			if (dir == LEFT) ghosts_[i].setPosition(800 - map_.getTileWidth(), ghosts_[i].getPosition().y);
			if (dir == RIGHT) ghosts_[i].setPosition(0, ghosts_[i].getPosition().y);
		}
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

	for (size_t i = 0; i < ghosts_.size(); i++)
		window_.draw(ghosts_[i].getSprite());

	window_.draw(pacman_.getSprite());
}

