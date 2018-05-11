#include "BlueScreen.hpp"
#include <SFML/Graphics.hpp>


void BlueScreen::drawState() const
{
    window_.clear();
    auto size = window_.getSize();
    sf::RectangleShape rect(window_.mapPixelToCoords({(int) size.x, (int)size.y}));
    rect.setFillColor(sf::Color::Blue);
    window_.draw(rect);
    window_.display();
}
