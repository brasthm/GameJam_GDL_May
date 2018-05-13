#include "ScreenGameOver.hpp"

ScreenGameOver::ScreenGameOver(sf::RenderWindow& window) : Screen{ window }
{
	font_.loadFromFile("../../font/upheavtt.ttf");
	renderT_.create(800, 600);
}

std::unique_ptr<Screen> ScreenGameOver::execute()
{
	return std::unique_ptr<Screen>(nullptr);
}
