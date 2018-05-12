#ifndef DIRECTIONAL_SRIPTE_H
#define DIRECTIONAL_SRIPTE_H

#include "Sprite.h"


class DirectionalSprite
{
	private :
		std::vector<Sprite> sprites_;
		orientation_t current_;
	public:
		DirectionalSprite();

		void setTexture(orientation_t dir, std::string path);

		void move(float deltaX, float deltaY);
		void setPosition(float deltaX, float deltaY);

		void setDelay(int delay);
		void setDelay(int delay, int moveDelay, int moveAnimDelay);
		void setTileSize(size_t w, size_t h);

		void updateMap(Map *m);
		void changeDirection(orientation_t dir);

		void update();

		sf::Sprite getSprite() const { return sprites_[current_].getSprite(); };
		const orientation_t getDirection() { return current_; };
		const bool isOOB() { return sprites_[current_].isOOB(); };

		sf::Vector2f getPosition() { return sprites_[current_].getPosition(); };
};

#endif // !DIRECTIONNAL_SRPTIE_H

