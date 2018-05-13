#include"DJ.hpp"

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
	AllTrack_[0].setTrack("../../sound/decompte.wav", "decompte");
	//AllTrack_[1].setTrack("", ""); free space
	AllTrack_[2].setTrack("../../sound/Defeat.wav", "defeat2");
	AllTrack_[3].setTrack("../../sound/Hit.wav", "Hit");
	AllTrack_[4].setTrack("../../sound/Jump.wav", "Jump");
	AllTrack_[5].setTrack("../../sound/Pickup_banana.wav", "Pickup_banana");
	AllTrack_[6].setTrack("../../sound/Pickup_rotten_banana.wav", "Pickup_rotten_banana");
	AllTrack_[7].setTrack("../../sound/Powerup.wav", "Powerup");
	AllTrack_[8].setTrack("../../sound/Shoot.wav", "Shoot");
	AllTrack_[9].setTrack("../../sound/victory.wav", "victory");
	AllTrack_[10].setTrack("../../sound/waka.wav", "waka");
	AllTrack_[12].setTrack("../../sound/piou8bits.wav", "piou");
	AllTrack_[11].setTrack("../../sound/Laser_Shoot.wav", "Laser");

	AllTrack_[10].getSound().setVolume(25);
	AllTrack_[11].getSound().setVolume(50);

	playMusicForever();
}

void DJ::playMusicForever() 
{
	music_.openFromFile("../../music/Komiku_-_64_-_First_Dance.ogg");
	music_.play();
	music_.setLoop(true);
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