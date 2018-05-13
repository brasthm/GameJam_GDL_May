#ifndef GAMESWITCHER_HPP
#define GAMESWITCHER_HPP

#include "Games/Games.hpp"
#include "Screen.hpp"

struct Step
{
	int id;
	sf::Time duration, gameDuration;
	float timeMultiplier;
};

class GameSwitcher : public Screen
{
    public:
		explicit GameSwitcher(sf::RenderWindow& window, DJ& dj);
        std::unique_ptr<Screen> execute() override;
        std::unique_ptr<Game> randomGame();
        std::unique_ptr<Game> randomGame(const std::unique_ptr<Game>& previous);

        std::optional<std::unique_ptr<Screen>> transition(std::unique_ptr<Game> nextGame, bool isSpeedUp);
        
		void renderUI();
		void computeUI(sf::Time elapsed, sf::Time frame);
    private:
        std::unique_ptr<Game> currentGame_;
        sf::RenderTexture renderT_;

		sf::Font font_;
		sf::Text scoreUI_;
		std::vector<sf::Texture> countdownText_;
		sf::Texture speedUp_;
		sf::RectangleShape progressBar_;
		sf::Sprite countdown_;

		int animationTrigger_ = -1;

		std::vector<Step> steps_;
		int currentStep_ = 0;
};


#endif //GAMESWITCHER_HPP
