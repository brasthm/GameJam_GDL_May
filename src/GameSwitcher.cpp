#include "GameSwitcher.hpp"
#include <random>


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
        
        if(gameClock.getElapsedTime() > sf::seconds(10) || !continueGame) 
        {
			//HACK désactivation temporaire
            auto result = transition(randomGame(currentGame_));
            if(result)
                return std::move(*result);
            gameClock.restart();
            frameClock.restart();
            continueGame = true;
        }
        else
        {
            renderT_.clear();
            continueGame = currentGame_->computeFrame(frameClock.restart(), score_);
            currentGame_->drawState();
            renderT_.display();
            
            gameScreen.setTexture(renderT_.getTexture());
            
            window_.clear();
            window_.draw(gameScreen);
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
            
            renderT_.clear();
            currentGame_->drawState();
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

            renderT_.clear();
            nextGame->drawState();
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
