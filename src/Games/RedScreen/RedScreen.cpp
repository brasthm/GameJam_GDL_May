#include "RedScreen.hpp"
#include <SFML/Graphics.hpp>


void RedScreen::drawState(sf::Sprite &countdown) const
{
    auto size = window_.getSize();
    sf::RectangleShape rect(window_.mapPixelToCoords({(int) size.x, (int)size.y}));
    rect.setFillColor(sf::Color::Red);
    window_.draw(rect);
}
