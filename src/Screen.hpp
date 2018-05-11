#ifndef ECRAN_H
#define ECRAN_H

#include <optional>
#include <stack>
#include <memory>
#include <vector>
#include <map>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


void adapt_viewport(sf::RenderWindow& window);

class Screen
{
    public:
        explicit Screen(sf::RenderWindow& window);
        virtual ~Screen() = default;

        virtual std::unique_ptr<Screen> execute() = 0;
        virtual std::optional<std::unique_ptr<Screen>> gestionEvent(const sf::Event& event);

    protected:
        sf::RenderWindow& window_;
};

#endif //ECRAN_H
