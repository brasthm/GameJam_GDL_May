#ifndef SPRITE_H
#define SPRITE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>


class Sprite
{
	protected :
		std::vector<sf::Texture> textures_;
		sf::Sprite sprite_;
		sf::Vector2f position_;
		sf::Time delay_;
		sf::Clock clock_;
		size_t nb_textures_ = 0, current_ = 0;
	public:
		Sprite();

		void setTexture(std::string path);

		void move(float deltaX, float deltaY);
		void setPosition(float deltaX, float deltaY);


		void setDelay(int delay);

		void update();

		sf::Sprite getSprite() const { return sprite_; };
};



#endif SPRITE_H