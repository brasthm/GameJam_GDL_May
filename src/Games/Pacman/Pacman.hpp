#ifndef PACMAN_HPP
#define PACMAN_HPP


#include "../Game.hpp"
#include "../../Map.h"
#include "../../Sprite.h"
#include "../../DirectionalSprite.h"

#include <random>

class Pacman : public Game
{
	private:
		Map map_;
		DirectionalSprite pacman_;
		std::vector<sf::Vector2i> gums_;
		std::vector<sf::Vector2i> noeuds_;

		sf::Texture gum_;

		Sprite fruit_;
		bool fruitAlive_ = true;

		sf::Keyboard keyboard_;


		void fillGum();
		void collision();
		void ia();
		orientation_t getDirection(sf::Vector2i source, sf::Vector2i destination);
	public :
		Map getMap() { return map_; };
		Sprite fantomeRouge_;

		explicit Pacman(sf::RenderWindow& window);

		void computeFrame(const sf::Time& elapsedTime) override;
		void drawState() const override;
};


#endif // !PACMAN_HPP
