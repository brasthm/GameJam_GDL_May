#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>


class Map
{
	private:
		std::vector<std::vector<int>> map_;
		std::vector<sf::Texture> tileset_;
		size_t tileWidth_, tileHeight_;
		size_t nbMaxWidth_, nbMaxHeight_;
		size_t nb_textures_;
	public:
		Map();
		
		void addTexture(std::string path);
		void loadFromFile(std::string path);
		void setTileSize(size_t width, size_t height);
		void draw(sf::RenderWindow &window) const;

		const bool isBlank(float x, float y) { int i = x / tileWidth_, j = y / tileHeight_; return map_[j][i] == 0; };
		const sf::Vector2i getTile(float x, float y) { return { (int) (x / tileWidth_), (int) (y / tileHeight_ )}; };
		const sf::Vector2f getPosition(int i, int j) { return { (float)(i*tileWidth_), float(j*tileHeight_) }; };
		
};


#endif // MAP_H