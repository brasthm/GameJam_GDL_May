#ifndef DJ_HPP
#define DJ_HPP
#include <SFML/Audio.hpp>
#include<vector>
#include<string>

class Track {
private : 
	sf::SoundBuffer buffer_;
	sf::Sound sound_;
	std::string name_;
public :
	Track();
	void setTrack(std::string filepath, std::string name);
	const std::string& getName() const{ return name_; }
	sf::Sound& getSound() { return sound_; }
};

class DJ 
{
private : 
	std::vector<Track> AllTrack_;
public :
	DJ();
	std::vector<Track>& getAllTrack(){ return AllTrack_; }
};

#endif //DJ_HPP