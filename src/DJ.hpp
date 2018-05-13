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

class Music {
private:
	sf::Music music_;
	std::string name_;
public:
	Music();
	void setMusic(std::string filepath, std::string name);
	const std::string& getName() const { return name_; }
	sf::Music& getMusic() { return music_; }
};


class DJ 
{
private : 
	std::vector<Track> AllTrack_;
	std::vector<Music> AllMusic_;
public :
	DJ();
	std::vector<Track>& getAllTrack(){ return AllTrack_; }
	std::vector<Music>& getAllMusic() { return AllMusic_; }
};

#endif //DJ_HPP