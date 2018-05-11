#include "Screen.hpp"

Screen::Screen(sf::RenderWindow& window) :  window_{ window }
{}

std::optional<std::unique_ptr<Screen>> Screen::gestionEvent(const sf::Event& event)
{
    if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Delete))
        return std::make_optional(std::unique_ptr<Screen>(nullptr));
    else if(event.type == sf::Event::Resized) 
    {
        adapt_viewport(window_);
        return std::nullopt;
    }

    return std::nullopt;
}


void adapt_viewport(sf::RenderWindow& window)
{
    auto view = window.getView();
    auto size = window.getSize();

    float ratio = (float)size.x / size.y;
    float wanted_ratio = 800.0f / 600.0f;

    if(ratio > wanted_ratio)
    {
        float proportion = wanted_ratio / ratio;
        view.setViewport({(1-proportion)/2, 0, proportion, 1});
    }
    else
    {
        float proportion = ratio / wanted_ratio;
        view.setViewport({0, (1-proportion)/2, 1, proportion});
    }

    window.setView(view);
}