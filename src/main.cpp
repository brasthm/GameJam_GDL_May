#include <SFML/Graphics.hpp>
#include "GameSwitcher.hpp"


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");

	sf::Music music;
	DJ dj(music);

    std::unique_ptr<Screen> current_screen(new GameSwitcher(window, dj));
    while(current_screen)
        current_screen = current_screen->execute();
    
    return 0;
}