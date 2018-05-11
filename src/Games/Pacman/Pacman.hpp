#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "../Game.hpp"
#include "../../Sprite.h"

class Pacman : public Game
{
	private:
		Sprite pacman_;
		float speedX_ = 100, speedY_ = 0;
	public :
		explicit Pacman(sf::RenderWindow& window);

		void computeFrame(const sf::Time& elapsedTime) override;
		void drawState() const override;
};


#endif // !PACMAN_HPP
