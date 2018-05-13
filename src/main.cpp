#include <SFML/Graphics.hpp>
#include "GameSwitcher.hpp"


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");

	DJ dj;
	sf::Music music;
	music.openFromFile("../../music/Komiku_-_64_-_First_Dance.ogg");
	music.play();
	music.setLoop(true);

    std::unique_ptr<Screen> current_screen(new GameSwitcher(window, dj));
    while(current_screen)
        current_screen = current_screen->execute();
    
    return 0;
}