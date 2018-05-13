#ifndef BOXHEAD_HPP
#define BOXHEAD_HPP

#include "../Game.hpp"
#include "../../Sprite.h"

struct zombiecomparator
{
	bool operator()(const sf::Sprite& a, const sf::Sprite& b)
	{
		return a.getPosition().y < b.getPosition().y;
	}
};

class Bullet {
private:
	sf::Sprite bullet_;
	char bulletSens_;
public:
	Bullet();
	void assign(char, float, float, sf::Texture&);
	char getSens()const { return bulletSens_; }
	void move(float, const sf::Time& elapsedTime);
	const sf::Sprite& getSprite() const { return bullet_; };
};

class Boxhead : public Game
{
private:
	sf::Sprite player_;
	std::vector<sf::Texture> TPlayerULDR_;
	char sens_;

	std::vector<sf::Sprite> zombieVect_;
	std::vector<sf::Texture> TzombieULDR_;

	std::vector<sf::Sprite> wallVect_;
	sf::Texture Twall_;
	
	sf::Time shootDelay_ = sf::milliseconds(200);
	sf::Time shootProgression_;
	sf::Time spawnRate_ = sf::seconds(5);
	sf::Time spawnProgression_;
	sf::Time deltaRate_ = sf::milliseconds(100);
	sf::Time delatProgression_;

	std::vector<Bullet> bulletVect_;
	sf::Texture Tbullet_;

	std::vector<sf::Sprite> bloodVect_;
	sf::Texture Tblood_;

	sf::Keyboard keyboard_;

	float speedX_ = 200, speedY_ = 200;
	float speedZombieX_ = 100, speedZombieY_ = 100;
	float speedWeapon_ = 600;
	
	bool lose_ = false;
public:
	explicit Boxhead(sf::RenderTarget& window, DJ& dj);
	bool computeFrame(const sf::Time& elapsedTime, int& score) override;
	void drawState(sf::Sprite &countdown) const override;
	void movePlayer(const sf::Time& elapsedTime);
	bool colli(char, sf::Sprite&);
	void shoot(const sf::Time& elapsedTime, char sens);
	void computeBullet(const sf::Time& elapsedTime, int& score);
	bool colliBullet();
	void moveZombie(const sf::Time& elapsedTime, int& score);
	char directZombie(sf::Sprite&);
	void genererZombie(int);
	void spawnZombie(const sf::Time& elapsedTime);
};

#endif // !BOXHEAD_HPP