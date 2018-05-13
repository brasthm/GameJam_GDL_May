#include "ScreenGameOver.hpp"
#include "constants.hpp"


ScreenGameOver::ScreenGameOver(sf::RenderWindow& window, DJ& dj) : Screen{ window, dj }
{
	font_.loadFromFile(location "font/upheavtt.ttf");
	renderT_.create(800, 600);
}

std::unique_ptr<Screen> ScreenGameOver::execute()
{
	return std::unique_ptr<Screen>(nullptr);
}
