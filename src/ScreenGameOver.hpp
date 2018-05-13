#ifndef SCREEN_GAME_OVER_HPP
#define SCREEN_GAME_OVER_HPP

#include "Screen.hpp"

class ScreenGameOver : public Screen
{
public:
	explicit ScreenGameOver(sf::RenderWindow& window);
	std::unique_ptr<Screen> execute() override;


private:
	sf::RenderTexture renderT_;
	sf::Font font_;
};

#endif // !SCREEN_GAME_OVER_HPP


