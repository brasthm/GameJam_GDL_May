#include "GameSwitcher.hpp"
#include <random>


GameSwitcher::GameSwitcher(sf::RenderWindow & window) : Screen{ window } 
{
	score_ = 0;

	currentGame_ = randomGame();
	renderT_.create(800, 600);

	font_.loadFromFile("../../font/upheavtt.ttf");
	scoreUI_.setFont(font_);
	scoreUI_.setCharacterSize(32);
	scoreUI_.setFillColor(sf::Color::White);
	scoreUI_.setPosition(10, 0);

	countdownText_.emplace_back();
	countdownText_.back().loadFromFile("../../img/ui/3.png");
	countdownText_.emplace_back();
	countdownText_.back().loadFromFile("../../img/ui/2.png");
	countdownText_.emplace_back();
	countdownText_.back().loadFromFile("../../img/ui/1.png");

	progressBar_.setFillColor(sf::Color::White);
	progressBar_.setPosition(0, 590);

	gameDuration_ = 3.f;
}

std::unique_ptr<Screen> GameSwitcher::execute()
{
    sf::Clock frameClock;
    sf::Clock gameClock;
    
    bool continueGame = true;
    
    sf::Sprite gameScreen;
	
    while(window_.isOpen())
    {
        sf::Event event;
        while(window_.pollEvent(event))
        {
            auto next = gestionEvent(event);
            if(next)
                return std::move(*next);
        }
        
        if(gameClock.getElapsedTime() > sf::seconds(gameDuration_) || !continueGame)
        {
			//HACK désactivation temporaire
            auto result = transition(randomGame(currentGame_));
            if(result)
                return std::move(*result);
            gameClock.restart();
            frameClock.restart();
            continueGame = true;

			animationTrigger_ = -1;
			countdown_.setTexture(countdownText_[0]);
			countdown_.setColor({ 255, 255, 255, 255 });
			countdown_.scale({ 1.f / countdown_.getScale().x, 1.f / countdown_.getScale().y });

        }
        else
        {
            renderT_.clear();
			computeUI(gameClock.getElapsedTime(), frameClock.getElapsedTime());
            continueGame = currentGame_->computeFrame(frameClock.restart(), score_);
			currentGame_->drawState(countdown_);
            renderT_.display();
            
            gameScreen.setTexture(renderT_.getTexture());
            
            window_.clear();
            window_.draw(gameScreen);
			renderUI();
            window_.display();
        }
        
        sf::sleep(sf::milliseconds(10));
    }
    
    return std::unique_ptr<Screen>(nullptr);
}

std::unique_ptr<Game> GameSwitcher::randomGame()
{
    static std::random_device rd;
    static std::default_random_engine gen(rd());
    std::uniform_int_distribution<size_t> distrib(0, game_list.size()-1);
    auto& game_instanciator = *game_list[distrib(gen)];
    return game_instanciator(renderT_);
}

std::unique_ptr<Game> GameSwitcher::randomGame(const std::unique_ptr<Game>& previous)
{
    std::unique_ptr<Game> ptr;
    do
    {
        ptr = std::move(randomGame());
    } while(typeid(*ptr) == typeid(*previous));
    
    return ptr;
}

std::optional<std::unique_ptr<Screen>> GameSwitcher::transition(std::unique_ptr<Game> nextGame)
{
    sf::Time progression;
    sf::Time openTime = sf::milliseconds(250);
    sf::Time closeTime = sf::milliseconds(250);
    
    sf::Clock frameClock;
    
    while(window_.isOpen())
    {
        sf::Event event;
        while(window_.pollEvent(event))
        {
            auto next = gestionEvent(event);
            if(next)
                return std::move(*next);
        }
        
        progression += frameClock.restart();
        
        if(progression < closeTime) {
            float ratio = progression.asSeconds() / closeTime.asSeconds();
            sf::Sprite lastGameScreen;
			sf::Sprite blank;
            renderT_.clear();
            currentGame_->drawState(blank);
            renderT_.display();
            
            lastGameScreen.setTexture(renderT_.getTexture());
            
            sf::RectangleShape rectL({400*ratio, 600});
            sf::RectangleShape rectR({400*ratio, 600});
            rectR.setPosition({800- 400*ratio, 0});
            rectL.setFillColor(sf::Color::Black);
            rectR.setFillColor(sf::Color::Black);
            
            window_.clear();
            window_.draw(lastGameScreen);
            window_.draw(rectL);
            window_.draw(rectR);
            window_.display();
        }
        else if(progression < closeTime+openTime) {
            float ratio = (progression-closeTime).asSeconds() / openTime.asSeconds();
            sf::Sprite lastGameScreen;
			sf::Sprite blank;
            renderT_.clear();
            nextGame->drawState(blank);
            renderT_.display();

            lastGameScreen.setTexture(renderT_.getTexture());

            sf::RectangleShape rectL({400*(1-ratio), 600});
            sf::RectangleShape rectR({400*(1-ratio), 600});
            rectR.setPosition({800- 400*(1-ratio), 0});
            rectL.setFillColor(sf::Color::Black);
            rectR.setFillColor(sf::Color::Black);

            window_.clear();
            window_.draw(lastGameScreen);
            window_.draw(rectL);
            window_.draw(rectR);
            window_.display();
        }
        else
        {
            currentGame_ = std::move(nextGame);
            return std::nullopt;
        }
    }

    return std::nullopt;
}

void GameSwitcher::renderUI()
{
	window_.draw(scoreUI_);
	window_.draw(progressBar_);
}

void GameSwitcher::computeUI(sf::Time elapsed, sf::Time frame)
{
	if (animationTrigger_ == 1 && elapsed > sf::seconds(gameDuration_ - 1 * 0.5))
	{
		animationTrigger_ = 2;
		countdown_.setTexture(countdownText_[2]);
		countdown_.setColor({ 255, 255, 255, 255 });
		countdown_.scale({ 1.f / countdown_.getScale().x, 1.f / countdown_.getScale().y });
	}
	else if (animationTrigger_ == 0 && elapsed > sf::seconds(gameDuration_ - 2 * 0.5))
	{
		animationTrigger_ = 1;
		countdown_.setTexture(countdownText_[1]);
		countdown_.setColor({ 255, 255, 255, 255 });
		countdown_.scale({ 1.f / countdown_.getScale().x, 1.f / countdown_.getScale().y });
	}
	else if (animationTrigger_ == -1 && elapsed > sf::seconds(gameDuration_ - 3 * 0.5))
	{
		animationTrigger_ = 0;
		countdown_.setTexture(countdownText_[0]);
		countdown_.setColor({ 255, 255, 255, 255 });
		countdown_.scale({ 1.f / countdown_.getScale().x, 1.f / countdown_.getScale().y });
	}
	else if (elapsed < sf::seconds(gameDuration_ - 3 * 0.5))
	{
		animationTrigger_ = -1;
		countdown_.setColor({ 0, 0, 0, 0 });
	}

	if (animationTrigger_ != -1)
	{
		float scaleValue = 1 - 1 * frame.asSeconds();
		countdown_.scale(scaleValue, scaleValue);
		countdown_.setPosition((800 - countdown_.getGlobalBounds().width) / 2, (600 - countdown_.getGlobalBounds().height) / 2);
	}

	progressBar_.setSize({ 800.f * (1 - elapsed.asSeconds() / gameDuration_), 10 });
	scoreUI_.setString("Score : " + std::to_string(score_));
}

