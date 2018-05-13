#include "MainMenu.h"

MainMenu::MainMenu(sf::RenderWindow & window, DJ & dj) : Screen{ window, dj }
{
	bg_.loadFromFile("../../img/mainMenu/bg.png");
	cursor_.loadFromFile("../../img/pacman/apple.png");

	current_ = 0;
	maxOptions_ = 3;
}

std::unique_ptr<Screen> MainMenu::execute()
{
	sf::Sprite fond, cursor;
	fond.setTexture(bg_);
	cursor.setTexture(cursor_);

	cursor.setPosition(250, 345);

	while (window_.isOpen())
	{
		sf::Event event;
		while (window_.pollEvent(event))
		{
			auto next = gestionEvent(event);
			if (next)
				return std::move(*next);

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
			{
				switch (current_)
				{
					case 0:
						return std::unique_ptr<Screen>(new GameSwitcher(window_, dj_));
						break;
					case 1:
						break;
					case 2:
						return std::unique_ptr<Screen>();
						break;
				}
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
			{
				current_--;
				if (current_ < 0) current_ = 0;
				else cursor.move(0, -45);
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
			{
				current_++;
				if (current_ >= maxOptions_) current_ = maxOptions_ - 1;
				else cursor.move(0, 45);
			}
		}


		window_.clear();
		window_.draw(fond);
		window_.draw(cursor);
		window_.display();

		sf::sleep(sf::milliseconds(10));
	}
	return std::unique_ptr<Screen>();
}
