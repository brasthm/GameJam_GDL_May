#include <SFML/Graphics.hpp>
#include "MainMenu.h"


// Global variable is evil
int score = 0;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "10 seconds classics");
	sf::Image icone;

	icone.loadFromFile(location "img/pacman/apple.png");

	window.setIcon(icone.getSize().x, icone.getSize().y, icone.getPixelsPtr());

	sf::Music music;
	DJ dj(music);

    std::unique_ptr<Screen> current_screen(new MainMenu(window, dj));
    while(current_screen)
        current_screen = current_screen->execute();
    
    return 0;
}