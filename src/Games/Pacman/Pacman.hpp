#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "../Game.hpp"
#include "../../Map.h"
#include "../../Sprite.h"
#include "../../DirectionalSprite.h"

class Pacman : public Game
{
	private:
		Map map_;
		DirectionalSprite pacman_;
		std::vector<Sprite> gum_;

		sf::Keyboard keyboard_;
		float speedX_, speedY_, maxSpeed_;

		void fillGum();
	public :
		explicit Pacman(sf::RenderWindow& window);

		void computeFrame(const sf::Time& elapsedTime) override;
		void drawState() const override;
};


#endif // !PACMAN_HPP
