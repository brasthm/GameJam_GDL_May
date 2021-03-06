#include "Pacman.hpp"
#include "../../constants.hpp"


void Pacman::fillGum()
{
	
	for (size_t i = 0; i < 800 / map_.getTileWidth(); i++)
	{
		for (size_t j = 0; j < 600 / map_.getTileHeight(); j++)
		{
			if (map_.getValue(i, j) == 0)
			{
				if (!(j == 5 && i == 0) && !(j == 5 && i == 1) && !(j == 9 && i == 0) && !(j == 9 && i == 1) && 
					!(j == 5 && i == 18) && !(j == 5 && i == 19) && !(j == 9 && i == 18) && !(j == 9 && i == 19) &&
					!(j == 1 && i == 1) && !(j == 13 && i == 1) && !(j == 1 && i == 18) && !(j == 13 && i == 18))
					gums_.emplace_back(i, j);
			}
		}
	}
	megaGums_.emplace_back(1, 1);
	megaGums_.emplace_back(1, 13);
	megaGums_.emplace_back(18, 1);
	megaGums_.emplace_back(18, 13);
}

void Pacman::collision(int &score)
{
	for (size_t i = 0; i < gums_.size(); i++)
	{
		if (map_.getTile(pacman_.getPosition().x + 20, pacman_.getPosition().y + 20) == gums_[i])
		{
			gums_.erase(gums_.begin() + i);
			i--;
			score += 50;
			//Dj_.getAllTrack()[3].getSound().play();
		}
	}

	for (size_t i = 0; i < megaGums_.size(); i++)
	{
		if (map_.getTile(pacman_.getPosition().x + 20, pacman_.getPosition().y + 20) == megaGums_[i])
		{
			megaGums_.erase(megaGums_.begin() + i);
			i--;

			isInvincible = true;
			invincible_ = sf::seconds(0);

			for (size_t i = 0; i < ghosts_.size(); i++)
			{
				ghosts_[i].setDelay(100000);
				ghosts_[i].applyTexture(1);
			}
			Dj_.getAllTrack()[7].getSound().play();
			score += 200;
		}
	}

	if (fruitAlive_ && map_.getTile(pacman_.getPosition().x + 20, pacman_.getPosition().y + 20) == (map_.getTile(fruit_.getPosition().x + 20, fruit_.getPosition().y + 20)))
	{
		fruitAlive_ = false;
		Dj_.getAllTrack()[5].getSound().play();
		score += 300;
	}

	for (size_t i = 0; i < ghosts_.size(); i++)
	{
		if (ghostAlive_[i] && map_.getTile(pacman_.getPosition().x + 20, pacman_.getPosition().y + 20) == (map_.getTile(ghosts_[i].getPosition().x + 20, ghosts_[i].getPosition().y + 20)))
		{
			if (!isInvincible)
			{
				dead_ = true;
				score -= 500;
				Dj_.getAllTrack()[2].getSound().play();
			}
			else
			{
				ghostAlive_[i] = false;
				score += 500;
				Dj_.getAllTrack()[5].getSound().play();
			}
				
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

Pacman::Pacman(sf::RenderTarget & window, DJ& dj) : Game{ window , dj}
{
	invincible_ = sf::seconds(0);
	map_.setTileSize(40, 40);

	map_.addTexture("blank");
	map_.addTexture(location "img/pacman/wall_h.png");
	map_.addTexture(location "img/pacman/wall_v.png");
	map_.addTexture(location "img/pacman/wall_corner_topleft.png");
	map_.addTexture(location "img/pacman/wall_corner_topright.png");
	map_.addTexture(location "img/pacman/wall_corner_bottomleft.png");
	map_.addTexture(location "img/pacman/wall_corner_bottomright.png");
	map_.addTexture(location "img/pacman/wall_end_left.png");
	map_.addTexture(location "img/pacman/wall_end_right.png");
	map_.addTexture(location "img/pacman/wall_end_up.png");
	map_.addTexture(location "img/pacman/wall_end_down.png");
	map_.addTexture(location "img/pacman/wall_three_left.png");
	map_.addTexture(location "img/pacman/wall_three_right.png");
	map_.addTexture(location "img/pacman/wall_three_top.png");
	map_.addTexture(location "img/pacman/wall_three_bottom.png");
	map_.addTexture(location "img/pacman/wall_block.png");

	map_.loadFromFile(location "maps/pacman.txt");

	pacman_.setTexture(RIGHT, location "img/pacman/pacman1_right.png");
	pacman_.setTexture(RIGHT, location "img/pacman/pacman2_right.png");
	pacman_.setTexture(LEFT, location "img/pacman/pacman1_left.png");
	pacman_.setTexture(LEFT, location "img/pacman/pacman2_left.png");
	pacman_.setTexture(UP, location "img/pacman/pacman1_up.png");
	pacman_.setTexture(UP, location "img/pacman/pacman2_up.png");
	pacman_.setTexture(DOWN, location "img/pacman/pacman1_down.png");
	pacman_.setTexture(DOWN, location "img/pacman/pacman2_down.png");

	pacman_.updateMap(&map_);
	pacman_.setDelay(200, 200, 200);
	pacman_.setTileSize(40, 40);
	pacman_.setPosition(40, 40);

	gum_.loadFromFile(location "img/pacman/gum.png");
	megaGum_.loadFromFile(location "img/pacman/mega_gum.png");
	fillGum();

	static std::random_device rd;
	static std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> uniform(0, gums_.size() - 1), uniform2(0, 2);
	int n;

	n = uniform(gen);
	pacman_.setPosition(40 * gums_[n].x, 40 * gums_[n].y);
	gums_.erase(gums_.begin() + n);

	n = uniform(gen);
	ghosts_.emplace_back();
	ghosts_.back().setTexture(location "img/pacman/red_ghost.png");
	ghosts_.back().setTexture(location "img/pacman/rip_ghost.png");
	ghosts_.back().updateMap(&map_);
	ghosts_.back().setDelay(100000, 400, 400);
	ghosts_.back().setTileSize(40, 40);
	ghosts_.back().setPosition(40 * gums_[n].x, 40 * gums_[n].y);

	n = uniform(gen);
	ghosts_.emplace_back();
	ghosts_.back().setTexture(location "img/pacman/orange_ghost.png");
	ghosts_.back().setTexture(location "img/pacman/rip_ghost.png");
	ghosts_.back().updateMap(&map_);
	ghosts_.back().setDelay(100000, 400, 400);
	ghosts_.back().setTileSize(40, 40);
	ghosts_.back().setPosition(40 * gums_[n].x, 40 * gums_[n].y);

	n = uniform(gen);
	ghosts_.emplace_back();
	ghosts_.back().setTexture(location "img/pacman/blue_ghost.png");
	ghosts_.back().setTexture(location "img/pacman/rip_ghost.png");
	ghosts_.back().updateMap(&map_);
	ghosts_.back().setDelay(100000, 400, 400);
	ghosts_.back().setTileSize(40, 40);
	ghosts_.back().setPosition(40 * gums_[n].x, 40 * gums_[n].y);

	n = uniform(gen);
	ghosts_.emplace_back();
	ghosts_.back().setTexture(location "img/pacman/pink_ghost.png");
	ghosts_.back().setTexture(location "img/pacman/rip_ghost.png");
	ghosts_.back().updateMap(&map_);
	ghosts_.back().setDelay(100000, 400, 400);
	ghosts_.back().setTileSize(40, 40);
	ghosts_.back().setPosition(40 * gums_[n].x, 40 * gums_[n].y);

	for (size_t i = 0; i < ghosts_.size(); i++)
	{
		ghostAlive_.emplace_back(true);
		ghosts_[i].applyTexture(0);
	}

	switch (uniform2(gen))
	{
		case 0:
			fruit_.setTexture(location "img/pacman/apple.png");
			break;
		case 1:
			fruit_.setTexture(location "img/pacman/cherry.png");
			break;
		case 2:
			fruit_.setTexture(location "img/pacman/orange.png");
			break;
	}

	fruit_.applyTexture();

	n = uniform(gen);

	fruit_.setPos(gums_[n].x * map_.getTileWidth(), gums_[n].y* map_.getTileHeight());
	gums_.erase(gums_.begin() + n);
}

bool Pacman::computeFrame(const sf::Time & elapsedTime, int& score)
{
    if(dead_)
        return false;

	Dj_.play(10);
    
	if (isInvincible) invincible_ += elapsedTime;

	if (invincible_ > sf::seconds(2))
		for (size_t i = 0; i < ghosts_.size(); i++)
			ghosts_[i].setDelay(200);

	if (invincible_ > sf::seconds(4))
	{
		isInvincible = false;
		invincible_ = sf::seconds(0);

		for (size_t i = 0; i < ghosts_.size(); i++)
			ghosts_[i].setDelay(100000);

		for (size_t i = 0; i < ghosts_.size(); i++)
			ghosts_[i].applyTexture(0);
	}

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

	pacman_.update(elapsedTime);

	for (size_t i = 0; i < ghosts_.size(); i++)
		if (ghostAlive_[i])
			ghosts_[i].update(elapsedTime);

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

	collision(score);
	
	return true;
}

void Pacman::drawState(sf::Sprite &countdown) const
{
	map_.draw(window_);
	window_.draw(countdown);

	for (size_t i = 0; i < gums_.size(); i++)
	{
		sf::Sprite temp;

		temp.setTexture(gum_);
		temp.setPosition(gums_[i].x * map_.getTileWidth(), gums_[i].y * map_.getTileHeight());
		window_.draw(temp);
	}


	for (size_t i = 0; i < megaGums_.size(); i++)
	{
		sf::Sprite temp;

		temp.setTexture(megaGum_);
		temp.setPosition(megaGums_[i].x * map_.getTileWidth(), megaGums_[i].y * map_.getTileHeight());
		window_.draw(temp);
	}

	if(fruitAlive_)
		window_.draw(fruit_.getSprite());

	for (size_t i = 0; i < ghosts_.size(); i++)
		if(ghostAlive_[i])
			window_.draw(ghosts_[i].getSprite());

	window_.draw(pacman_.getSprite());
}

