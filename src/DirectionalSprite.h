#ifndef DIRECTIONAL_SRIPTE_H
#define DIRECTIONAL_SRIPTE_H

#include "Sprite.h"


enum orientation_t { UP, DOWN, LEFT, RIGHT };

class DirectionalSprite
{
	private :
		std::vector<Sprite> sprites_;
		orientation_t current_;
	public:
		DirectionalSprite();

		void setTexture(orientation_t dir, std::string path);

		void move(float deltaX, float deltaY);
		void move(Map &map, float deltaX, float deltaY);
		void setPosition(float deltaX, float deltaY);

		void setDelay(int delay);

		void changeDirection(orientation_t dir);

		void update();

		sf::Sprite getSprite() const { return sprites_[current_].getSprite(); };
		const orientation_t getDirection() { return current_; };
		const bool isOOB() { return sprites_[current_].isOOB(); };
};

#endif // !DIRECTIONNAL_SRPTIE_H

