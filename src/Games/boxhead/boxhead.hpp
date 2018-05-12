#ifndef BOXHEAD_HPP
#define BOXHEAD_HPP

#include "../Game.hpp"
#include "../../Sprite.h"

class Boxhead : public Game
{
private:
	sf::Sprite player_;
	std::vector<sf::Texture> TPlayerULDR_;

	std::vector<sf::Sprite> zombieVect_;
	std::vector<sf::Texture> TzombieULDR_;

	std::vector<sf::Sprite> wallVect_;
	sf::Texture Twall_;

	sf::Sprite bullet_;

	std::vector<sf::Sprite> bloodVect_;
	sf::Texture Tblood_;

	sf::Keyboard keyboard_;

	float speedX_ = 200, speedY_ = 200;
public:
	explicit Boxhead(sf::RenderWindow& window);
	void computeFrame(const sf::Time& elapsedTime) override;
	void drawState() const override;
	void movePlayer(const sf::Time& elapsedTime);
	bool colli(char);
};





#endif // !BOXHEAD_HPP