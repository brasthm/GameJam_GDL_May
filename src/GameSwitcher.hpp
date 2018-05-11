#ifndef GAMESWITCHER_HPP
#define GAMESWITCHER_HPP

#include "Games/Games.hpp"
#include "Screen.hpp"

class GameSwitcher : public Screen
{
    public:
        explicit GameSwitcher(sf::RenderWindow& window) : Screen{window} {
            currentGame_ = randomGame();
        }
        std::unique_ptr<Screen> execute() override;
        std::unique_ptr<Game> randomGame();
        
    private:
        std::unique_ptr<Game> currentGame_;
};


#endif //GAMESWITCHER_HPP
