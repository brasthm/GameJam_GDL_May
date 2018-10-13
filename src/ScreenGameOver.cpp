#include "ScreenGameOver.hpp"
#include "constants.hpp"
#include "MainMenu.h"
#include <fstream>
#include <iostream>
#include <cctype>
#include <algorithm>


ScreenGameOver::ScreenGameOver(sf::RenderWindow& window, DJ& dj) : Screen{ window, dj }
{
	font_.loadFromFile(location "font/upheavtt.ttf");

	bg_.loadFromFile(location "img/mainMenu/gameover.png");
	
	std::fstream scoreFile(location "data/score.txt");
	if(scoreFile.is_open()) {
	    int s;
	    std::string name;
	    while(scoreFile >> s && std::getline(scoreFile, name))
        {
			if (name.find_first_not_of(' ') != std::string::npos)
			{
				name = name.substr(name.find_first_not_of(' '));
				scores_.emplace_back(s, name);
			}
        }
	}
	
	scores_.emplace_back(score, "<placeholder>");
	std::sort(scores_.begin(), scores_.end(), [](std::pair<int, std::string> a, std::pair<int, std::string> b) { return a.first > b.first; });
}

ScreenGameOver::~ScreenGameOver()
{
	std::ofstream file(location "data/score.txt", std::ios::trunc);

	if (file.is_open())
	{
		for (auto &elem : scores_)
			file << elem.first << " " << elem.second << std::endl;
	}
}

std::unique_ptr<Screen> ScreenGameOver::execute()
{
	sf::Sprite fond;
	sf::Text playerName;
	auto emplacement = std::find_if(scores_.begin(), scores_.end(), [](const std::pair<int, std::string> &elem) { return elem.second == "<placeholder>"; });
	std::string name = "";

	emplacement->second = "";
	
	fond.setTexture(bg_);
	playerName.setFont(font_);
	playerName.setCharacterSize(38);
	playerName.setPosition(290, 295);

	dj_.playMusicForever(location "music/menu.ogg");

	while (window_.isOpen())
	{
		sf::Event event;
		while (window_.pollEvent(event))
		{
			auto next = gestionEvent(event);
			if (next)
				return std::move(*next);

			if (event.type == sf::Event::TextEntered)
			{
				if (!std::iscntrl(event.text.unicode) && name.size() < 10) {
					name.push_back((char)event.text.unicode);
					emplacement->second = name;
				}
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace && name.size() != 0)
			{
				name.pop_back();
				emplacement->second = name;
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return && name.find_first_not_of(' ') != std::string::npos)
				return std::unique_ptr<Screen>(new MainMenu(window_, dj_));
		}

		playerName.setPosition(290, 287);
		playerName.setString(name + "_");

		

		window_.clear();
		window_.draw(fond);
		window_.draw(playerName);

		playerName.setString("Score : " + std::to_string(score));
		playerName.setPosition((800 - playerName.getGlobalBounds().width) / 2, 200);
		window_.draw(playerName);

		for (size_t i = 0; i < std::min((size_t)5,scores_.size()); i++)
		{
			playerName.setPosition(150, 380 + i * 40);
			playerName.setString(std::to_string(i + 1) + (i ? "" : " ") + " - " + scores_[i].second);
			window_.draw(playerName);

			playerName.setPosition(500, 380 + i * 40);
			playerName.setString(std::to_string(scores_[i].first));
			window_.draw(playerName);
		}
		window_.display();

	}
	return std::unique_ptr<Screen>(nullptr);
}