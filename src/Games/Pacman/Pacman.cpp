#include "Pacman.hpp"

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

	pacman_.setPosition(40, 40);
	pacman_.setDelay(200);

	maxSpeed_ = 200;
	speedX_ = maxSpeed_;
	speedY_ = 0;
}

void Pacman::computeFrame(const sf::Time & elapsedTime)
{
	if (keyboard_.isKeyPressed(sf::Keyboard::Up))
	{
		speedX_ = 0;
		speedY_ = -maxSpeed_;
		pacman_.changeDirection(UP);
	}
	else if (keyboard_.isKeyPressed(sf::Keyboard::Down))
	{
		speedX_ = 0;
		speedY_ = maxSpeed_;
		pacman_.changeDirection(DOWN);
	}
	else if (keyboard_.isKeyPressed(sf::Keyboard::Left))
	{
		speedX_ = -maxSpeed_;
		speedY_ = 0;
		pacman_.changeDirection(LEFT);
	}
	else if (keyboard_.isKeyPressed(sf::Keyboard::Right))
	{
		speedX_ = maxSpeed_;
		speedY_ = 0;
		pacman_.changeDirection(RIGHT);
	}


	pacman_.move(speedX_ * elapsedTime.asSeconds(), speedY_ * elapsedTime.asSeconds());
	pacman_.update();

	if (pacman_.isOOB())
	{
		orientation_t dir = pacman_.getDirection();
		if (dir == UP) pacman_.move(0, 600);
		if (dir == DOWN) pacman_.move(0, -600);
		if (dir == LEFT) pacman_.move(800, 0);
		if (dir == RIGHT) pacman_.move(-800, 0);
	}
}

void Pacman::drawState() const 
{
	map_.draw(window_);
	window_.draw(pacman_.getSprite());
}
