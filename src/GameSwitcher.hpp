#ifndef GAMESWITCHER_HPP
#define GAMESWITCHER_HPP

#include "Games/Games.hpp"
#include "Screen.hpp"

class GameSwitcher : public Screen
{
    public:
		explicit GameSwitcher(sf::RenderWindow& window);
        std::unique_ptr<Screen> execute() override;
        std::unique_ptr<Game> randomGame();
        std::unique_ptr<Game> randomGame(const std::unique_ptr<Game>& previous);

        std::optional<std::unique_ptr<Screen>> transition(std::unique_ptr<Game> nextGame);
        
		void renderUI();
		void computeUI(sf::Time elapsed, sf::Time frame);
    private:
        std::unique_ptr<Game> currentGame_;
        int score_;
        sf::RenderTexture renderT_;

		sf::Font font_;
		sf::Text scoreUI_;
		std::vector<sf::Texture> countdownText_;
		sf::RectangleShape progressBar_;
		sf::Sprite countdown_;

		int animationTrigger_ = -1;
		int gameDuration_;

};


#endif //GAMESWITCHER_HPP
