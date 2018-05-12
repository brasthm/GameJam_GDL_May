#include "boxhead.hpp"
#include <iostream>

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
	
	sf::Sprite Szombie;
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

	clock_.restart();
	shootDelay_ = sf::milliseconds(200);

	//gene alea de zombie
	Szombie.setTexture(TzombieULDR_[1]);
	zombieVect_.push_back(Szombie);
	zombieVect_.push_back(Szombie);
	zombieVect_[0].setPosition(300, 300);
	zombieVect_[1].setPosition(600, 300);
}

bool Boxhead::colli(char sens) 
{
	switch (sens)
	{
	case'U':
		for (size_t i = 0; i < wallVect_.size(); i++)
			if (wallVect_[i].getGlobalBounds().intersects(sf::FloatRect(player_.getPosition().x, player_.getPosition().y - 1, 40, 56)))
			{
				player_.setPosition(player_.getPosition().x, wallVect_[i].getPosition().y + 119);
				return true;
			}
		for (size_t i = 0; i < zombieVect_.size(); i++)
			if (zombieVect_[i].getGlobalBounds().intersects(sf::FloatRect(player_.getPosition().x, player_.getPosition().y - 1, 40, 56)))
			{
				player_.setPosition(player_.getPosition().x, zombieVect_[i].getPosition().y + 56);
				return true;
			}
		if (player_.getPosition().y < 0)
			return true;
		break;

	case'D':
		for (size_t i = 0; i < wallVect_.size(); i++)
			if (wallVect_[i].getGlobalBounds().intersects(sf::FloatRect(player_.getPosition().x, player_.getPosition().y + 1, 40, 56)))
			{
				player_.setPosition(player_.getPosition().x, wallVect_[i].getPosition().y - 56);
				return true;
			}
		for (size_t i = 0; i < zombieVect_.size(); i++)
			if (zombieVect_[i].getGlobalBounds().intersects(sf::FloatRect(player_.getPosition().x, player_.getPosition().y + 1, 40, 56)))
			{
				player_.setPosition(player_.getPosition().x, zombieVect_[i].getPosition().y - 56);
				return true;
			}
		if (player_.getPosition().y + 56 > 600) //+56 car on dessend et +56 car je verifie au pied
			return true;
		break;

	case'L':
		for (size_t i = 0; i < wallVect_.size(); i++)
			if (wallVect_[i].getGlobalBounds().intersects(sf::FloatRect(player_.getPosition().x - 1, player_.getPosition().y, 40, 56)))
			{
				player_.setPosition(wallVect_[i].getPosition().x + 104, player_.getPosition().y);
				return true;
			}
		for (size_t i = 0; i < zombieVect_.size(); i++)
			if (zombieVect_[i].getGlobalBounds().intersects(sf::FloatRect(player_.getPosition().x - 1, player_.getPosition().y, 40, 56)))
			{
				player_.setPosition(zombieVect_[i].getPosition().x + 40, player_.getPosition().y);
				return true;
			}
		if (player_.getPosition().x  < 0) 
			return true;
		break;

	case'R':
		for (size_t i = 0; i < wallVect_.size(); i++)
			if (wallVect_[i].getGlobalBounds().intersects(sf::FloatRect(player_.getPosition().x + 1, player_.getPosition().y, 40, 56)))
			{
				player_.setPosition(wallVect_[i].getPosition().x - 40, player_.getPosition().y);
				return true;
			}
		for (size_t i = 0; i < zombieVect_.size(); i++)
			if (zombieVect_[i].getGlobalBounds().intersects(sf::FloatRect(player_.getPosition().x + 1, player_.getPosition().y, 40, 56)))
			{
				player_.setPosition(zombieVect_[i].getPosition().x - 40, player_.getPosition().y);
				return true;
			}
		if (player_.getPosition().x + 40 > 800)
			return true;
		break;

	default:
		break;
	}
	return false;
}

void Boxhead::movePlayer(const sf::Time& elapsedTime)
{
	if (keyboard_.isKeyPressed(sf::Keyboard::Left))
		if (!colli('L'))
		{
			player_.move(-speedX_ * elapsedTime.asSeconds(), 0);
			player_.setTexture(TPlayerULDR_[1]);
			sens_ = 'L';
		}
		
	if (keyboard_.isKeyPressed(sf::Keyboard::Right))
		if (!colli('R'))
		{
			player_.move(speedX_ * elapsedTime.asSeconds(), 0);
			player_.setTexture(TPlayerULDR_[3]);
			sens_ = 'R';
		}

	if (keyboard_.isKeyPressed(sf::Keyboard::Up))
		if (!colli('U'))
		{
			player_.move(0, -speedY_ * elapsedTime.asSeconds());
			player_.setTexture(TPlayerULDR_[0]);
			sens_ = 'U';
		}
	
	if (keyboard_.isKeyPressed(sf::Keyboard::Down))
		if (!colli('D'))
		{
			player_.move(0, speedY_ * elapsedTime.asSeconds());
			player_.setTexture(TPlayerULDR_[2]);
			sens_ = 'D';
		}
}

void Boxhead::shoot(const sf::Time& elapsedTime, char sens)
{
	Bullet bullet;
	if (keyboard_.isKeyPressed(sf::Keyboard::Space))
		if(elapsedTime>=shootDelay_)
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
			clock_.restart();
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
				blood.setPosition((*itz).getPosition().x, (*itz).getPosition().y + 16);
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

	//true si collie zombie uniquement
	if (colliBullet())
	{ }//mettre du score
		
}

void Boxhead::computeFrame(const sf::Time& elapsedTime)
{
	//fct de move player
	movePlayer(elapsedTime);
	//fct de move zombie
	//ftc de shoot
	shoot(clock_.getElapsedTime(), sens_);
	computeBullet(elapsedTime);
}

void Boxhead::drawState()const
{
	auto size = window_.getSize();
	sf::RectangleShape rect(window_.mapPixelToCoords({ (int)size.x, (int)size.y }));
	rect.setFillColor(sf::Color::White);
	window_.draw(rect);
	for (size_t i = 0; i < 6; i++)
		window_.draw(wallVect_[i]);
	for (size_t i = 0; i < bloodVect_.size(); i++)
		window_.draw(bloodVect_[i]);
	for (size_t i = 0; i < zombieVect_.size(); i++)
		window_.draw(zombieVect_[i]);
	for (size_t i = 0; i < bulletVect_.size(); i++)
		window_.draw(bulletVect_[i].getSprite());
	window_.draw(player_);
}
