#include"DJ.hpp"
#include "constants.hpp"


Track::Track() 
{
}

void Track::setTrack(std::string filepath, std::string name) 
{
	buffer_.loadFromFile(filepath);
	sound_.setBuffer(buffer_);
	name_ = name;
}

DJ::DJ(sf::Music& m) : music_(m)
{
	AllTrack_.resize(20);
	AllTrack_[0].setTrack(location "sound/decompte.wav", "decompte");
	//AllTrack_[1].setTrack("", ""); free space
	AllTrack_[2].setTrack(location "sound/Defeat.wav", "defeat2");
	AllTrack_[3].setTrack(location "sound/Hit.wav", "Hit");
	AllTrack_[4].setTrack(location "sound/Jump.wav", "Jump");
	AllTrack_[5].setTrack(location "sound/Pickup_banana.wav", "Pickup_banana");
	AllTrack_[6].setTrack(location "sound/Pickup_rotten_banana.wav", "Pickup_rotten_banana");
	AllTrack_[7].setTrack(location "sound/Powerup.wav", "Powerup");
	AllTrack_[8].setTrack(location "sound/Shoot.wav", "Shoot");
	AllTrack_[9].setTrack(location "sound/victory.wav", "victory");
	AllTrack_[10].setTrack(location "sound/waka.wav", "waka");
	AllTrack_[12].setTrack(location "sound/piou8bits.wav", "piou");
	AllTrack_[11].setTrack(location "sound/Laser_Shoot.wav", "Laser");
	AllTrack_[13].setTrack(location "sound/speed_up.wav", "Laser");

	AllTrack_[10].getSound().setVolume(25);
	AllTrack_[11].getSound().setVolume(50);

}

void DJ::playMusicForever(std::string path) 
{
	music_.openFromFile(path);
	music_.play();
	music_.setLoop(true);

	sf::Listener listener;
	listener.setGlobalVolume(50);
}

void DJ::play(size_t n, bool forced)
{
	if (forced)
		AllTrack_[n].getSound().play();
	else
	{
		if (AllTrack_[n].getSound().getStatus() != sf::Sound::Playing) AllTrack_[n].getSound().play();
	}
}