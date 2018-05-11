#include <SFML/Graphics.hpp>
#include "GameSwitcher.hpp"


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    
    GameSwitcher game_switcher(window);
    game_switcher.execute();
    
    return 0;
}