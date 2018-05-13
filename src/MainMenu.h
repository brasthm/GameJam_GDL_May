#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Screen.hpp"
#include "GameSwitcher.hpp"

class MainMenu : public Screen
{
	public:
		explicit MainMenu(sf::RenderWindow& window, DJ& dj);
		std::unique_ptr<Screen> execute() override;
	private:
		sf::Texture bg_, cursor_;
		int current_;
		int maxOptions_;
};


#endif // !MAIN_MENU_H
