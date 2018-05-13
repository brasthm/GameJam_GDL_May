#include "boxhead.hpp"
#include <iostream>
#include <random>
#include<cmath>

Bullet::Bullet() 
{
}

void Bullet::assign(char sens, float x, float y, sf::Texture& Tbullet) 
{
	bullet_.setTexture(Tbullet);
	bullet_.setPosition(x, y);
	bulletSens_ = sens;
}

void Bullet::move(float speed, const sf::Time& elapsedTime)
{
	switch (bulletSens_)
	{
	case 'U':
		bullet_.move(0, -speed * elapsedTime.asSeconds());
		break;
	case 'L':
		bullet_.move(-speed * elapsedTime.asSeconds(), 0);
		break;
	case 'R':
		bullet_.move(+speed * elapsedTime.asSeconds(), 0);
		break;
	case 'D':
		bullet_.move(0, +speed * elapsedTime.asSeconds());
		break;
	default:
		break;
	}
}

Boxhead::Boxhead(sf::RenderWindow& window) : Game{ window }
{
	sf::Texture TplayerD;
	TplayerD.loadFromFile("../../img/boxhead/playerDown.png");
	sf::Texture TplayerL;
	TplayerL.loadFromFile("../../img/boxhead/playerLeft.png");
	sf::Texture TplayerR;
	TplayerR.loadFromFile("../../img/boxhead/playerRight.png");
	sf::Texture TplayerU;
	TplayerU.loadFromFile("../../img/boxhead/playerUp.png");
	TPlayerULDR_.push_back(TplayerU); 
	TPlayerULDR_.push_back(TplayerL); 
	TPlayerULDR_.push_back(TplayerD); 
	TPlayerULDR_.push_back(TplayerR);
	player_.setTexture(TPlayerULDR_[2]);
	sens_ = 'D';
	player_.setPosition(400, 300);

	sf::Sprite Swall;
	Twall_.loadFromFile("../../img/boxhead/wall.png");
	Swall.setTexture(Twall_);
	for (size_t i = 0; i < 6; i++)
		wallVect_.push_back(Swall);
	wallVect_[0].setPosition(56, 56);
	wallVect_[1].setPosition(43 * 8, 56);
	wallVect_[2].setPosition(79 * 8, 56);
	wallVect_[3].setPosition(56, 52 * 8);
	wallVect_[4].setPosition(43 * 8, 52 * 8);
	wallVect_[5].setPosition(79 * 8, 52 * 8);

	Tblood_.loadFromFile("../../img/boxhead/blood.png");
	Tbullet_.loadFromFile("../../img/boxhead/bullet.png");
	
	
	sf::Texture TzombieU;
	TzombieU.loadFromFile("../../img/boxhead/zombieUp.png");
	sf::Texture TzombieR;
	TzombieR.loadFromFile("../../img/boxhead/zombieRight.png");
	sf::Texture TzombieD;
	TzombieD.loadFromFile("../../img/boxhead/zombieDown.png");
	sf::Texture TzombieL;
	TzombieL.loadFromFile("../../img/boxhead/zombieLeft.png");
	TzombieULDR_.push_back(TzombieU);
	TzombieULDR_.push_back(TzombieL);
	TzombieULDR_.push_back(TzombieD);
	TzombieULDR_.push_back(TzombieR);

	genererZombie(5);

}

void Boxhead::genererZombie(int nbzombie) 
{
	sf::Sprite Szombie;
	int spawn;
	int delat = 20;
	static std::random_device rd;
	static std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> uniform(0,7);

	for (int i = 0; i < nbzombie; i++)
	{
		spawn = uniform(gen);
		switch (spawn)
		{
		case 0:
			Szombie.setTexture(TzombieULDR_[3]);
			Szombie.setPosition(8, 26 * 8 +delat*i);
			break;
		case 1:
			Szombie.setTexture(TzombieULDR_[2]);
			Szombie.setPosition(28 * 8, 3 * 8 + delat * i);
			break;
		case 2:
			Szombie.setTexture(TzombieULDR_[2]);
			Szombie.setPosition(65 * 8, 3 * 8 + delat * i);
			break;
		case 3:
			Szombie.setTexture(TzombieULDR_[1]);
			Szombie.setPosition(94 * 8, 26 * 8 + delat * i);
			break;
		case 4:
			Szombie.setTexture(TzombieULDR_[1]);
			Szombie.setPosition(94 * 8, 42 * 8 + delat * i);
			break;
		case 5:
			Szombie.setTexture(TzombieULDR_[0]);
			Szombie.setPosition(66 * 8 + delat * i, 67 * 8);
			break;
		case 6:
			Szombie.setTexture(TzombieULDR_[0]);
			Szombie.setPosition(27 * 8 + delat * i, 67 * 8 );
			break;
		case 7:
			Szombie.setTexture(TzombieULDR_[3]);
			Szombie.setPosition(8, 40 * 8 + delat * i);
			break;
		default:
			break;
		}
		zombieVect_.push_back(Szombie);
	}
}

bool Boxhead::colli(char sens, sf::Sprite& entity) 
{
	switch (sens)
	{
	case'U':
		for (size_t i = 0; i < wallVect_.size(); i++)
			if (wallVect_[i].getGlobalBounds().intersects(sf::FloatRect(entity.getPosition().x, entity.getPosition().y - 2, 40, 56)))
				return true;

		if (entity.getPosition().y < 0)
			return true;
		break;

	case'D':
		for (size_t i = 0; i < wallVect_.size(); i++)
			if (wallVect_[i].getGlobalBounds().intersects(sf::FloatRect(entity.getPosition().x, entity.getPosition().y + 2, 40, 56)))
				return true;

		if (entity.getPosition().y + 56 > 600) //+56 car on dessend et +56 car je verifie au pied
			return true;
		break;

	case'L':
		for (size_t i = 0; i < wallVect_.size(); i++)
			if (wallVect_[i].getGlobalBounds().intersects(sf::FloatRect(entity.getPosition().x - 2, entity.getPosition().y, 40, 56)))
				return true;
		
		if (entity.getPosition().x  < 0)
			return true;
		break;

	case'R':
		for (size_t i = 0; i < wallVect_.size(); i++)
			if (wallVect_[i].getGlobalBounds().intersects(sf::FloatRect(entity.getPosition().x + 2, entity.getPosition().y, 40, 56)))
				return true;
		
		if (entity.getPosition().x + 40 > 800)
			return true;
		break;

	default:
		break;
	}
	return false;
}

void Boxhead::movePlayer(const sf::Time& elapsedTime)
{
	sf::Vector2f posbefore = player_.getPosition();

	if (keyboard_.isKeyPressed(sf::Keyboard::Left))
		if (!colli('L', player_))
		{
			player_.move(-speedX_ * elapsedTime.asSeconds(), 0);
			player_.setTexture(TPlayerULDR_[1]);
			sens_ = 'L';
		}
		else
			player_.setPosition(posbefore);
		
	if (keyboard_.isKeyPressed(sf::Keyboard::Right))
		if (!colli('R', player_))
		{
			player_.move(speedX_ * elapsedTime.asSeconds(), 0);
			player_.setTexture(TPlayerULDR_[3]);
			sens_ = 'R';
		}
		else 
			player_.setPosition(posbefore);

	if (keyboard_.isKeyPressed(sf::Keyboard::Up))
		if (!colli('U', player_))
		{
			player_.move(0, -speedY_ * elapsedTime.asSeconds());
			player_.setTexture(TPlayerULDR_[0]);
			sens_ = 'U';
		}
		else
			player_.setPosition(posbefore);
	
	if (keyboard_.isKeyPressed(sf::Keyboard::Down))
		if (!colli('D', player_))
		{
			player_.move(0, speedY_ * elapsedTime.asSeconds());
			player_.setTexture(TPlayerULDR_[2]);
			sens_ = 'D';
		}
		else 
			player_.setPosition(posbefore);
}

void Boxhead::shoot(const sf::Time& elapsedTime, char sens)
{
	Bullet bullet;
	shootProgression_ += elapsedTime;
	if (keyboard_.isKeyPressed(sf::Keyboard::Space))
		if(shootProgression_ >=shootDelay_)
		{
			switch (sens)
			{
			case 'U':
				bullet.assign(sens, player_.getPosition().x + 32, player_.getPosition().y+8, Tbullet_);
				break;
			case 'L':
				bullet.assign(sens, player_.getPosition().x -8, player_.getPosition().y + 24, Tbullet_);
				break;
			case 'D':
				bullet.assign(sens, player_.getPosition().x, player_.getPosition().y + 32, Tbullet_);
				break;
			case 'R':
				bullet.assign(sens, player_.getPosition().x + 48, player_.getPosition().y + 24, Tbullet_);
				break;
			default:
				break;
			}
			shootProgression_ = sf::Time::Zero;
			bulletVect_.push_back(bullet);
		}
}

bool Boxhead::colliBullet() 
{
	sf::Sprite blood;
	blood.setTexture(Tblood_);
	
	for (std::vector<Bullet>::iterator itb = bulletVect_.begin(); itb != bulletVect_.end(); itb++)
	{
		for (std::vector<sf::Sprite>::iterator itz = zombieVect_.begin(); itz != zombieVect_.end(); itz++)
		{
			if ((*itz).getGlobalBounds().intersects((*itb).getSprite().getGlobalBounds()))
			{
				blood.setPosition(round(((*itz).getPosition().x)/8)*8, round(((*itz).getPosition().y + 16)/8)*8);
				bloodVect_.push_back(blood);
				zombieVect_.erase(itz);
				bulletVect_.erase(itb);
				return true;
			}
		}
		for (std::vector<sf::Sprite>::iterator itw = wallVect_.begin(); itw != wallVect_.end(); itw++)
		{
			if ((*itw).getGlobalBounds().intersects((*itb).getSprite().getGlobalBounds()))
			{
				bulletVect_.erase(itb);
				return false;
			}
		}
		if (((*itb).getSprite().getPosition().x <= -40) || ((*itb).getSprite().getPosition().x >= 840))
		{
			bulletVect_.erase(itb);
			return false;
		}
		if (((*itb).getSprite().getPosition().y <= -40) || ((*itb).getSprite().getPosition().y >= 640))
		{
			bulletVect_.erase(itb);
			return false;
		}
	}
	return false;
}

void Boxhead::computeBullet(const sf::Time& elapsedTime)
{
	for (size_t i = 0; i < bulletVect_.size(); i++)
		bulletVect_[i].move(speedWeapon_, elapsedTime);

	if (colliBullet())
	{ }//mettre du score
		
}

char Boxhead::directZombie(sf::Sprite& zombie) 
{
	
	if(abs(player_.getPosition().x - zombie.getPosition().x) > abs(player_.getPosition().y - zombie.getPosition().y))
	{
		if (player_.getPosition().x + 40 < zombie.getPosition().x)
			return 'L';
		if (player_.getPosition().x - 40 > zombie.getPosition().x)
			return 'R';
		if (player_.getPosition().y + 56 < zombie.getPosition().y)
			return 'U';
		if (player_.getPosition().y - 56 > zombie.getPosition().y)
			return 'D';
		else
			return 'C';
	}
	else 
	{
		if (player_.getPosition().y + 56 < zombie.getPosition().y)
			return 'U';
		if (player_.getPosition().y - 56 > zombie.getPosition().y)
			return 'D';
		if (player_.getPosition().x + 40 < zombie.getPosition().x)
			return 'L';
		if (player_.getPosition().x - 40 > zombie.getPosition().x)
			return 'R';
		else
			return 'C';
	}
	
}

void Boxhead::moveZombie(const sf::Time& elapsedTime)
{
	sf::Vector2f delta;
	static std::random_device rd;
	static std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> uniform(-5 , 5);
	sf::Vector2f posbefore;

	delatProgression_ += elapsedTime;

	for (size_t i = 0; i < zombieVect_.size(); i++)
	{
		posbefore = zombieVect_[i].getPosition();
		if(delatProgression_ > deltaRate_)
		{
			delta.x = uniform(gen);
			delta.y = uniform(gen);
			zombieVect_[i].move(delta);
		}
	
		switch (directZombie(zombieVect_[i]))
		{
		case 'U':
			if (!colli('U', zombieVect_[i]))
			{
				zombieVect_[i].move(0, -speedZombieY_ * elapsedTime.asSeconds());
				zombieVect_[i].setTexture(TzombieULDR_[0]);
			}
			else if (!colli('L', zombieVect_[i]))
			{
				zombieVect_[i].move(-speedZombieX_ * elapsedTime.asSeconds(), 0);
				zombieVect_[i].setTexture(TzombieULDR_[1]);
			}
			else if (!colli('R', zombieVect_[i]))
			{
				zombieVect_[i].move(+speedZombieX_ * elapsedTime.asSeconds() * 2, 0);
				zombieVect_[i].setTexture(TzombieULDR_[3]);
			}
			else
				zombieVect_[i].setPosition(posbefore);
			break;

		case 'L':
			if (!colli('L', zombieVect_[i]))
			{
				zombieVect_[i].move(-speedZombieX_ * elapsedTime.asSeconds(), 0);
				zombieVect_[i].setTexture(TzombieULDR_[1]);
			}
			else if (!colli('U', zombieVect_[i]))
			{
				zombieVect_[i].move(0, -speedZombieY_ * elapsedTime.asSeconds());
				zombieVect_[i].setTexture(TzombieULDR_[0]);
			}
			else if (!colli('D', zombieVect_[i]))
			{
				zombieVect_[i].move(0, +speedZombieY_ * elapsedTime.asSeconds() * 2);
				zombieVect_[i].setTexture(TzombieULDR_[2]);
			}
			else
				zombieVect_[i].setPosition(posbefore);
			break;

		case 'D':
			if (!colli('D', zombieVect_[i]))
			{
				zombieVect_[i].move(0, +speedZombieY_ * elapsedTime.asSeconds());
				zombieVect_[i].setTexture(TzombieULDR_[2]);
			}
			else if (!colli('L', zombieVect_[i]))
			{
				zombieVect_[i].move(-speedZombieX_ * elapsedTime.asSeconds(), 0);
				zombieVect_[i].setTexture(TzombieULDR_[1]);
			}
			else if (!colli('R', zombieVect_[i]))
			{
				zombieVect_[i].move(+speedZombieX_ * elapsedTime.asSeconds()*2, 0);
				zombieVect_[i].setTexture(TzombieULDR_[3]);
			}
			else
				zombieVect_[i].setPosition(posbefore);
			break;

		case 'R':
			if (!colli('R', zombieVect_[i]))
			{
				zombieVect_[i].move(+speedZombieX_ * elapsedTime.asSeconds(), 0);
				zombieVect_[i].setTexture(TzombieULDR_[3]);
			}
			else if (!colli('U', zombieVect_[i]))
			{
				zombieVect_[i].move(0, -speedZombieY_ * elapsedTime.asSeconds());
				zombieVect_[i].setTexture(TzombieULDR_[0]);
			}
			else if (!colli('D', zombieVect_[i]))
			{
				zombieVect_[i].move(0, +speedZombieY_ * elapsedTime.asSeconds()*2);
				zombieVect_[i].setTexture(TzombieULDR_[2]);
			}
			else
				zombieVect_[i].setPosition(posbefore);
			break;

		case 'C':
			//player lose score
			
				break;
		default:
			break;
		}

	}
	if(delatProgression_>deltaRate_)
		delatProgression_ = sf::Time::Zero;
}

void Boxhead::spawnZombie(const sf::Time& elapsedTime)
{
	spawnProgression_ += elapsedTime;
	if (spawnProgression_ >= spawnRate_)
	{
		genererZombie(5);
		spawnProgression_ = sf::Time::Zero;
	}
}

void Boxhead::computeFrame(const sf::Time& elapsedTime)
{
	movePlayer(elapsedTime);
	shoot(elapsedTime, sens_);
	computeBullet(elapsedTime);
	moveZombie(elapsedTime);
	spawnZombie(elapsedTime);
	std::sort(zombieVect_.begin(), zombieVect_.end(), zombiecomparator());
}

void Boxhead::drawState()const
{
	size_t i = 0;
	auto size = window_.getSize();
	sf::RectangleShape rect(window_.mapPixelToCoords({ (int)size.x, (int)size.y }));
	rect.setFillColor(sf::Color(102,51,0,255));
	window_.draw(rect);
	for (size_t i = 0; i < bloodVect_.size(); i++)
		window_.draw(bloodVect_[i]);
	for (size_t i = 0; i < 6; i++)
		window_.draw(wallVect_[i]);
	for (i = 0; (i < zombieVect_.size())&&(zombieVect_[i].getPosition().y <= player_.getPosition().y); i++)
		window_.draw(zombieVect_[i]);
	window_.draw(player_);
	for (; i < zombieVect_.size(); i++)
		window_.draw(zombieVect_[i]);
	for (size_t i = 0; i < bulletVect_.size(); i++)
		window_.draw(bulletVect_[i].getSprite());
	
	
}
