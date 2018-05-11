#include <SFML/Graphics.hpp>
#include "GameSwitcher.hpp"


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    
    std::unique_ptr<Screen> current_screen(new GameSwitcher(window));
    while(current_screen)
        current_screen = current_screen->execute();
    
    return 0;
}