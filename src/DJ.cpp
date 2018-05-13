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
	AllTrack_.resize(12);
	AllTrack_[0].setTrack("../../sound/decompte1_2.wav", "decompte1_2");
	AllTrack_[1].setTrack("../../sound/decompte3.wav", "decompte3");
	AllTrack_[2].setTrack("../../sound/Defeat.wav", "defeat2");
	AllTrack_[3].setTrack("../../sound/Hit.wav", "Hit");
	AllTrack_[4].setTrack("../../sound/Jump.wav", "Jump");
	AllTrack_[5].setTrack("../../sound/Pickup_banana.wav", "Pickup_banana");
	AllTrack_[6].setTrack("../../sound/Pickup_rotten_banana.wav", "Pickup_rotten_banana");
	AllTrack_[7].setTrack("../../sound/Powerup.wav", "Powerup");
	AllTrack_[8].setTrack("../../sound/Shoot.wav", "Shoot");
	AllTrack_[9].setTrack("../../sound/victory.wav", "victory");
	AllTrack_[10].setTrack("../../sound/waka.wav", "waka");
	AllTrack_[11].setTrack("../../sound/Laser_Shoot.wav", "victory");

	AllTrack_[10].getSound().setVolume(25);

	music_.openFromFile("../../music/Komiku_-_64_-_First_Dance.ogg");
	music_.play();
	music_.setLoop(true);
}
