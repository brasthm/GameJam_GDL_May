#ifndef SPRITE_H
#define SPRITE_H

#include "Map.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <string>

enum orientation_t { UP, RIGHT, DOWN, LEFT };


class Sprite
{
	protected :
		std::vector<sf::Texture> textures_;
		sf::Sprite sprite_;
		sf::Vector2f position_, prevPosition_, nextPosition_;
		sf::Time delay_, moveDelay_, moveAnimDelay_;
		sf::Clock clock_, clockMove_, clockMoveAnim_;
		size_t nb_textures_ = 0, current_ = 0;
		size_t tileWidth_, tileHeight_;

		bool auto_, iaReady_ = true;
		orientation_t direction_, nextDirection_;
		float speed_;

		void setNextPosition();

		Map *map_;

	public:
		Sprite();
		~Sprite();

		void setTexture(std::string path);

		void move(float deltaX, float deltaY);
		void setPosition(float deltaX, float deltaY);
		void setPos(float deltaX, float deltaY);
		void setPrevPosition(float deltaX, float deltaY);
		void setNextPosition(float deltaX, float deltaY);
		void setClcok(sf::Clock);

		void setDelay(int delay);
		void setDelay(int delay, int moveDelay, int moveAnimDelay);
		void setTileSize(size_t w, size_t h);

		void changeDirection(orientation_t dir);

		void applyTexture();
		void applyTexture(size_t n);

		void updateMap(Map *m) { map_ = m; };

		orientation_t update();

		sf::Sprite getSprite() const { return sprite_; };
		sf::Vector2f getPosition() const { return position_; };
		sf::Vector2f getPrevPosition() const { return prevPosition_; };
		sf::Vector2f getNextPosition() const { return nextPosition_; };
		sf::Clock getClock() const { return clockMoveAnim_; };
		sf::Vector2i getBackTile(float x, float y);
		
		orientation_t getDirection() { return direction_; };

		const bool isOOB();
		bool isIAReady() { return iaReady_; };
};



#endif SPRITE_H