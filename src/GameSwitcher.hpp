#ifndef GAMESWITCHER_HPP
#define GAMESWITCHER_HPP

#include "Games/Games.hpp"
#include "Screen.hpp"

class GameSwitcher : public Screen
{
    public:
        explicit GameSwitcher(sf::RenderWindow& window) : Screen{window}, score_{0} {
            currentGame_ = randomGame();
            renderT_.create(800, 600);
        }
        std::unique_ptr<Screen> execute() override;
        std::unique_ptr<Game> randomGame();
        std::unique_ptr<Game> randomGame(const std::unique_ptr<Game>& previous);

        std::optional<std::unique_ptr<Screen>> transition(std::unique_ptr<Game> nextGame);
        
    private:
        std::unique_ptr<Game> currentGame_;
        int score_;
        sf::RenderTexture renderT_;
};


#endif //GAMESWITCHER_HPP
