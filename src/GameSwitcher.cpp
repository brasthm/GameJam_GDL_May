#include "GameSwitcher.hpp"
#include <random>
#include "constants.hpp"


GameSwitcher::GameSwitcher(sf::RenderWindow & window, DJ& dj) : Screen{ window, dj}, score_{ 0 } 
{
	currentGame_ = randomGame();
	renderT_.create(800, 600);

	font_.loadFromFile(location "font/upheavtt.ttf");
	scoreUI_.setFont(font_);
	scoreUI_.setCharacterSize(32);
	scoreUI_.setFillColor(sf::Color::White);
	scoreUI_.setPosition(10, 0);

	countdownText_.emplace_back();
	countdownText_.back().loadFromFile(location "img/ui/3.png");
	countdownText_.emplace_back();
	countdownText_.back().loadFromFile(location "img/ui/2.png");
	countdownText_.emplace_back();
	countdownText_.back().loadFromFile(location "img/ui/1.png");

	speedUp_.loadFromFile(location "img/ui/speed_up.png");

	progressBar_.setFillColor(sf::Color::White);
	progressBar_.setPosition(0, 590);

	steps_.emplace_back();
	steps_.back().id = 0;
	steps_.back().duration = sf::seconds(30.f);
	steps_.back().gameDuration = sf::seconds(15.f);
	steps_.back().timeMultiplier = 1.f;

	steps_.emplace_back();
	steps_.back().id = 1;
	steps_.back().duration = sf::seconds(30.f);
	steps_.back().gameDuration = sf::seconds(10.f);
	steps_.back().timeMultiplier = 1.f;

	steps_.emplace_back();
	steps_.back().id = 2;
	steps_.back().duration = sf::seconds(30.f);
	steps_.back().gameDuration = sf::seconds(5.f);
	steps_.back().timeMultiplier = 1.2f;

	steps_.emplace_back();
	steps_.back().id = 3;
	steps_.back().duration = sf::seconds(15.f);
	steps_.back().gameDuration = sf::seconds(3.f);
	steps_.back().timeMultiplier = 1.5f;

	steps_.emplace_back();
	steps_.back().id = 4;
	steps_.back().duration = sf::seconds(10.f);
	steps_.back().gameDuration = sf::seconds(2.f);
	steps_.back().timeMultiplier = 1.8f;
}

std::unique_ptr<Screen> GameSwitcher::execute()
{
    sf::Clock frameClock;
    sf::Clock gameClock;
	sf::Clock globalClock;
    
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
        
        if(gameClock.getElapsedTime() > steps_[currentStep_].gameDuration || !continueGame)
        {
			bool isSpeedUp = false;
			if (globalClock.getElapsedTime() > steps_[currentStep_].duration)
			{
				currentStep_++;
				if (currentStep_ >= steps_.size())
					return std::unique_ptr<Screen>(nullptr);
				globalClock.restart();
				isSpeedUp = true;
			}
			//HACK désactivation temporaire
			if (continueGame)
				dj_.getAllTrack()[9].getSound().play();
            auto result = transition(randomGame(currentGame_), isSpeedUp);
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
            continueGame = currentGame_->computeFrame(frameClock.restart() * steps_[currentStep_].timeMultiplier, score_);
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
    return game_instanciator(renderT_, dj_);
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

std::optional<std::unique_ptr<Screen>> GameSwitcher::transition(std::unique_ptr<Game> nextGame, bool isSpeedUp)
{
    sf::Time progression;
    sf::Time openTime = sf::milliseconds(250);
    sf::Time closeTime = sf::milliseconds(250);
	sf::Time speedUpTime = isSpeedUp ? sf::milliseconds(500) : sf::milliseconds(0);
    
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
		else if (progression < closeTime + speedUpTime)
		{
			sf::Sprite speedUp;
			speedUp.setTexture(speedUp_);

			speedUp.setOrigin(speedUp.getGlobalBounds().width / 2, speedUp.getGlobalBounds().height / 2);
			speedUp.setPosition(400, 300);

			window_.clear();
			window_.draw(speedUp);
			window_.display();
		}
        else if(progression < closeTime+openTime+speedUpTime) {
            float ratio = (progression-closeTime- speedUpTime).asSeconds() / openTime.asSeconds();
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
	if (animationTrigger_ == 1 && elapsed > sf::seconds(steps_[currentStep_].gameDuration.asSeconds() - 1 * 0.5))
	{
		animationTrigger_ = 2;
		countdown_.setTexture(countdownText_[2]);
		countdown_.setColor({ 255, 255, 255, 255 });
		countdown_.scale({ 1.f / countdown_.getScale().x, 1.f / countdown_.getScale().y });
		dj_.getAllTrack()[0].getSound().play();
	}
	else if (animationTrigger_ == 0 && elapsed > sf::seconds(steps_[currentStep_].gameDuration.asSeconds() - 2 * 0.5))
	{
		animationTrigger_ = 1;
		countdown_.setTexture(countdownText_[1]);
		countdown_.setColor({ 255, 255, 255, 255 });
		countdown_.scale({ 1.f / countdown_.getScale().x, 1.f / countdown_.getScale().y });
		dj_.getAllTrack()[0].getSound().play();
	}
	else if (animationTrigger_ == -1 && elapsed > sf::seconds(steps_[currentStep_].gameDuration.asSeconds() - 3 * 0.5))
	{
		animationTrigger_ = 0;
		countdown_.setTexture(countdownText_[0]);
		countdown_.setColor({ 255, 255, 255, 255 });
		countdown_.scale({ 1.f / countdown_.getScale().x, 1.f / countdown_.getScale().y });
		dj_.getAllTrack()[0].getSound().play();
	}
	else if (elapsed < sf::seconds(steps_[currentStep_].gameDuration.asSeconds() - 3 * 0.5))
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

	progressBar_.setSize({ 800.f * (1 - elapsed.asSeconds() / steps_[currentStep_].gameDuration.asSeconds()), 10 });
	scoreUI_.setString("Score : " + std::to_string(score_));
}

