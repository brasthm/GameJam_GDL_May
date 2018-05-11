#ifndef GAMES_HPP
#define GAMES_HPP

#include "BlueScreen/BlueScreen.hpp"
#include "RedScreen/RedScreen.hpp"
#include "../utilities.hpp"

std::vector<std::unique_ptr<instanciator_base<Game, sf::RenderWindow&>>> init_game_list();

const std::vector<std::unique_ptr<instanciator_base<Game, sf::RenderWindow&>>> game_list = init_game_list();

#endif //GAMES_HPP
