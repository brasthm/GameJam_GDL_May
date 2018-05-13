#include <SFML/Graphics.hpp>
#include "MainMenu.h"


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "10 seconds classics");
	DJ dj;

    std::unique_ptr<Screen> current_screen(new MainMenu(window, dj));
    while(current_screen)
        current_screen = current_screen->execute();
    
    return 0;
}