#ifndef GAMES_HPP
#define GAMES_HPP

#include "BlueScreen/BlueScreen.hpp"
#include "RedScreen/RedScreen.hpp"
#include "Snake/Snake.hpp"
#include "Pacman/Pacman.hpp"
#include "../utilities.hpp"
#include "boxhead/boxhead.hpp"
#include "SpaceInvaders/SpaceInvaders.hpp"

std::vector<std::unique_ptr<instanciator_base<Game, sf::RenderWindow&>>> init_game_list();

const std::vector<std::unique_ptr<instanciator_base<Game, sf::RenderWindow&>>> game_list = init_game_list();

#endif //GAMES_HPP
