#include "Pacman.hpp"

Pacman::Pacman(sf::RenderWindow & window) : Game{ window }
{
	pacman_.setTexture("../../img/pacman/pacman1_right.png");
	pacman_.setTexture("../../img/pacman/pacman2_right.png");
	pacman_.setPosition(0, 0);
	pacman_.setDelay(200);
}

void Pacman::computeFrame(const sf::Time & elapsedTime)
{
	std::cerr << elapsedTime.asMilliseconds() << std::endl;
	pacman_.move(speedX_ * elapsedTime.asSeconds(), speedY_ * elapsedTime.asSeconds());
	pacman_.update();
}

void Pacman::drawState() const
{
	window_.draw(pacman_.getSprite());
}
