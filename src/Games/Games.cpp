#include "Games.hpp"


std::vector<std::unique_ptr<instanciator_base<Game, sf::RenderWindow&>>> init_game_list()
{
    std::vector<std::unique_ptr<instanciator_base<Game, sf::RenderWindow&>>> gl;
    //gl.emplace_back(new instanciator<Game, BlueScreen, sf::RenderWindow&>);
    //gl.emplace_back(new instanciator<Game, RedScreen, sf::RenderWindow&>);

	gl.emplace_back(new instanciator<Game, Pacman, sf::RenderWindow&>);

    return gl;
};