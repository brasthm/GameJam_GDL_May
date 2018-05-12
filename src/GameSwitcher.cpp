#include "GameSwitcher.hpp"
#include <random>


std::unique_ptr<Screen> GameSwitcher::execute()
{
    sf::Clock frameClock;
    sf::Clock gameClock;
    
    while(window_.isOpen())
    {
        sf::Event event;
        while(window_.pollEvent(event))
        {
            auto next = gestionEvent(event);
            if(next)
                return std::move(*next);
        }
        
        if(gameClock.getElapsedTime() > sf::seconds(30)) 
        {
            gameClock.restart();
            currentGame_ = std::move(randomGame());
            frameClock.restart();
        }
        else
        {
			window_.clear();
            currentGame_->computeFrame(frameClock.restart());
            currentGame_->drawState();
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
    return game_instanciator(window_);
}
