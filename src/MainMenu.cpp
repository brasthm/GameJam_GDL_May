#include "MainMenu.h"

MainMenu::MainMenu(sf::RenderWindow & window, DJ & dj) : Screen{ window, dj }
{
	bg_.loadFromFile(location "img/mainMenu/bg.png");
	credits_.loadFromFile(location "img/mainMenu/credits.png");
	cursor_.loadFromFile(location "img/pacman/apple.png");
	how2play_.loadFromFile(location "img/mainMenu/how2play.png");

	current_ = 0;
	maxOptions_ = 3;

	isCredits_ = false;
}

std::unique_ptr<Screen> MainMenu::execute()
{
	sf::Sprite fond, cursor;
	fond.setTexture(bg_);
	cursor.setTexture(cursor_);

	cursor.setPosition(250, 345);

	dj_.playMusicForever(location "music/menu.ogg");

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
				if (isCredits_)
				{
					isCredits_ = false;
					fond.setTexture(bg_);
				}
				else if (isHow2_)
				{
					dj_.playMusicForever(location "music/game.ogg");
					return std::unique_ptr<Screen>(new GameSwitcher(window_, dj_));
				}
				else
				{
					switch (current_)
					{
					case 0:
						isHow2_ = true;
						fond.setTexture(how2play_);
						break;
					case 1:
						isCredits_ = true;
						fond.setTexture(credits_);
						break;
					case 2:
						return std::unique_ptr<Screen>();
						break;
					}
				}
			}
			if (!isCredits_ && !isHow2_ && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
			{
				current_--;
				if (current_ < 0) current_ = 0;
				else cursor.move(0, -45);
			}

			if (!isCredits_ && !isHow2_ && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
			{
				current_++;
				if (current_ >= maxOptions_) current_ = maxOptions_ - 1;
				else cursor.move(0, 45);
			}
		}


		window_.clear();
		window_.draw(fond);
		if(!isCredits_ && !isHow2_) window_.draw(cursor);
		window_.display();

		sf::sleep(sf::milliseconds(10));
	}
	return std::unique_ptr<Screen>();
}
