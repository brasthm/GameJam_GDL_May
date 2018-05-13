#include "ScreenGameOver.hpp"

ScreenGameOver::ScreenGameOver(sf::RenderWindow& window) : Screen{ window }
{
	renderT_.create(800, 600);
	font_.loadFromFile("../../font/upheavtt.ttf");
}

std::unique_ptr<Screen> ScreenGameOver::execute()
{
	return std::unique_ptr<Screen>();
}
