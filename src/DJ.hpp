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
	sf::Music& music_;
public :
	DJ(sf::Music& m);
	std::vector<Track>& getAllTrack(){ return AllTrack_; }
	void playMusicForever();
	void play(size_t n, bool forced = false);

};

#endif //DJ_HPP