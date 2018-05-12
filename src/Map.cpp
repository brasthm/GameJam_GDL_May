#include "Map.h"

Map::Map()
{

}

void Map::addTexture(std::string path)
{
	tileset_.emplace_back();
	if (path != "blank")
	{
		if (!tileset_.back().loadFromFile(path))
			std::cerr << "Loading failed : " << path << " doesn't exist" << std::endl;
		else
			nb_textures_++;
	}
	else
		nb_textures_++;
	
}

void Map::loadFromFile(std::string path)
{
	std::ifstream file(path, std::ios::in); 

	if (file)
	{
		for (size_t i = 0; i < nbMaxHeight_; i++)
			for (size_t j = 0; j < nbMaxWidth_; j++)
				file >> map_[i][j];
			
		file.close();
	}
	else 
		std::cerr << "Loading failed : " << path << " doesn't exist" << std::endl;
}

void Map::setTileSize(size_t width, size_t height)
{
	tileWidth_ = width;
	tileHeight_ = height;

	nbMaxWidth_ = 800 / tileWidth_;
	nbMaxHeight_ = 600 / tileHeight_;

	if (800 % tileWidth_ != 0) nbMaxWidth_ += 1;
	if (600 % tileHeight_ != 0) nbMaxHeight_ += 1;

	for (size_t i = 0; i < map_.size(); i++)
		map_[i].clear();
	map_.clear();

	for (size_t i = 0; i < nbMaxHeight_; i++)
	{
		map_.emplace_back();
		for (size_t j = 0; j < nbMaxWidth_; j++)
			map_.back().emplace_back(0);
	}
}

void Map::draw(sf::RenderWindow &window) const
{
	for (size_t i = 0; i < nbMaxHeight_; i++)
	{
		for (size_t j = 0; j < nbMaxWidth_; j++)
		{
			sf::Sprite buffer;
			buffer.setTexture(tileset_[map_[i][j]]);
			buffer.setPosition(tileWidth_ * j, tileHeight_ * i);
			window.draw(buffer);
		}
	}
}

const bool Map::isBlank(float x, float y)
{
	int i = x / tileWidth_, j = y / tileHeight_; 

	if (i < 0 || i >= nbMaxWidth_ || j < 0 || j >= nbMaxHeight_)
		return true;

	return map_[j][i] == 0;
}
