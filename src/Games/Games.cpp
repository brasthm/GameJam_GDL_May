#include "Games.hpp"


std::vector<std::unique_ptr<instanciator_base<Game, sf::RenderTarget&, DJ& >>> init_game_list()
{
    std::vector<std::unique_ptr<instanciator_base<Game, sf::RenderTarget&>>> gl;
    //gl.emplace_back(new instanciator<Game, BlueScreen, sf::RenderTarget&>);
    //gl.emplace_back(new instanciator<Game, RedScreen, sf::RenderTarget&>);
	//gl.emplace_back(new instanciator<Game, Boxhead, sf::RenderTarget&>);
	gl.emplace_back(new instanciator<Game, SpaceInvaders, sf::RenderTarget&>);
	//gl.emplace_back(new instanciator<Game, Pacman, sf::RenderTarget&>);
	//gl.emplace_back(new instanciator<Game, Snake, sf::RenderTarget&>);
    //gl.emplace_back(new instanciator<Game, BananaHero, sf::RenderTarget&>);
    //gl.emplace_back(new instanciator<Game, Tetris, sf::RenderTarget&>);

    return gl;
};