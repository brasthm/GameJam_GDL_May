#ifndef SCREEN_GAME_OVER_HPP
#define SCREEN_GAME_OVER_HPP

#include "Screen.hpp"


extern int score;

class ScreenGameOver : public Screen
{
public:
	explicit ScreenGameOver(sf::RenderWindow& window, DJ& dj);
	~ScreenGameOver();
	std::unique_ptr<Screen> execute() override;


private:
	sf::Font font_;
	std::vector<std::pair<int, std::string>> scores_;
	int n_;
	sf::Texture bg_;
};

#endif // !SCREEN_GAME_OVER_HPP


