#include "ScreenGameOver.hpp"
#include "constants.hpp"
#include <fstream>


ScreenGameOver::ScreenGameOver(sf::RenderWindow& window, DJ& dj) : Screen{ window, dj }
{
	font_.loadFromFile(location "font/upheavtt.ttf");
	
	std::fstream scoreFile(location "data/score.txt");
	if(scoreFile.is_open()) {
	    int s;
	    std::string name;
	    while(scoreFile >> s && std::getline(scoreFile, name))
        {
            scores_.emplace_back(s, name);
        }
	}
	
	std::sort(scores_.begin(), scores_.end(), [](std::pair<int, std::string> a, std::pair<int, std::string> b) { return a.first < b.first; });
}

std::unique_ptr<Screen> ScreenGameOver::execute()
{
	return std::unique_ptr<Screen>(nullptr);
}
