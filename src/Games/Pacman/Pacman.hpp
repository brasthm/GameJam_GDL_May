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
		std::vector<sf::Vector2i> megaGums_;

		std::vector<Sprite> ghosts_;

		sf::Texture gum_, megaGum_;

		Sprite fruit_;
		bool fruitAlive_ = true;
		std::vector<bool> ghostAlive_;

		sf::Keyboard keyboard_;
		sf::Time invincible_;
		bool isInvincible = false;
		
		bool dead_ = false;

		void fillGum();
		void collision(int &score);
		void ia();
		orientation_t getDirection(sf::Vector2i source, sf::Vector2i destination);
	public :
		Map getMap() { return map_; };
		
		explicit Pacman(sf::RenderTarget& window, DJ& dj);

		bool computeFrame(const sf::Time& elapsedTime, int& score) override;
		void drawState(sf::Sprite &countdown) const override;
};


#endif // !PACMAN_HPP
