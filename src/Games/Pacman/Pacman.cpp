#include "Pacman.hpp"

void Pacman::fillGum()
{
	
	for (size_t i = 0; i < 800 / map_.getTileWidth(); i++)
	{
		for (size_t j = 0; j < 600 / map_.getTileHeight(); j++)
		{
			if (map_.getValue(i, j) == 0)
			{
				if (!(j == 5 && i == 0) && !(j == 5 && i == 1) && !(j == 9 && i == 0) && !(j == 9 && i == 1) && 
					!(j == 5 && i == 18) && !(j == 5 && i == 19) && !(j == 9 && i == 18) && !(j == 9 && i == 19))
					gums_.emplace_back(i, j);
			}
		}
	}
}

void Pacman::collision()
{
	for (size_t i = 0; i < gums_.size(); i++)
	{
		if (map_.getTile(pacman_.getPosition().x + 20, pacman_.getPosition().y + 20) == gums_[i])
		{
			gums_.erase(gums_.begin() + i);
			i--;
		}
	}

	if (fruitAlive_ && map_.getTile(pacman_.getPosition().x + 20, pacman_.getPosition().y + 20) == (map_.getTile(fruit_.getPosition().x + 20, fruit_.getPosition().y + 20)))
		fruitAlive_ = false;
}

void Pacman::ia()
{
	if (fantomeRouge_.isIAReady())
	{
		sf::Vector2i pacmanPosition = map_.getTile(pacman_.getPosition().x + 20, pacman_.getPosition().y + 20);
		sf::Vector2i fantomeRougePosition = map_.getTile(fantomeRouge_.getPosition().x + 20, fantomeRouge_.getPosition().y + 20);
		data_.length_function([this](sf::Vector2i a, sf::Vector2i b) // On fourni la fonction qui donne la distance entre deux noeuds
																	 // Elle prend en paramètre deux variables du type spécifié comme noeuds et retourne le type spécifié comme distance
		{
			sf::Vector2f pos = this->fantomeRouge_.getPosition();
			if (a == this->getMap().getTile(pos.x, pos.y) && b == this->fantomeRouge_.getBackTile(pos.x, pos.y))
				return std::numeric_limits<double>::infinity();

			if (a.x == b.x && abs(a.y - b.y) == 1
				|| a.x == b.x && abs(a.y - b.y) == 1)
				// Dans notre cas, les cases adjacentes sont espacées de 1
				return 1.0;
			else
				// Les autres sont inaccessibles directement
				return std::numeric_limits<double>::infinity(); 
		})
			.start_node(fantomeRougePosition);

		Dijkstra_solver<sf::Vector2i, double, std::vector<sf::Vector2i>> solver(data_);
		solver.solve();

		sf::Vector2i nextPos = solver.path_to(pacmanPosition).front();

		fantomeRouge_.changeDirection(getDirection(fantomeRougePosition, nextPos));
	}

}

orientation_t Pacman::getDirection(sf::Vector2i source, sf::Vector2i destination)
{
	sf::Vector2i delta = destination - source;

	if (delta.x > 0) return RIGHT;
	if (delta.x < 0) return LEFT;
	if (delta.y > 0) return DOWN;
	if (delta.x < 0) return UP;
}

Pacman::Pacman(sf::RenderWindow & window) : Game{ window }
{
	map_.setTileSize(40, 40);

	map_.addTexture("blank");
	map_.addTexture("../../img/pacman/wall_h.png");
	map_.addTexture("../../img/pacman/wall_v.png");
	map_.addTexture("../../img/pacman/wall_corner_topleft.png");
	map_.addTexture("../../img/pacman/wall_corner_topright.png");
	map_.addTexture("../../img/pacman/wall_corner_bottomleft.png");
	map_.addTexture("../../img/pacman/wall_corner_bottomright.png");
	map_.addTexture("../../img/pacman/wall_end_left.png");
	map_.addTexture("../../img/pacman/wall_end_right.png");
	map_.addTexture("../../img/pacman/wall_end_up.png");
	map_.addTexture("../../img/pacman/wall_end_down.png");
	map_.addTexture("../../img/pacman/wall_three_left.png");
	map_.addTexture("../../img/pacman/wall_three_right.png");
	map_.addTexture("../../img/pacman/wall_three_top.png");
	map_.addTexture("../../img/pacman/wall_three_bottom.png");
	map_.addTexture("../../img/pacman/wall_block.png");

	map_.loadFromFile("../../maps/pacman.txt");

	pacman_.setTexture(RIGHT, "../../img/pacman/pacman1_right.png");
	pacman_.setTexture(RIGHT, "../../img/pacman/pacman2_right.png");
	pacman_.setTexture(LEFT, "../../img/pacman/pacman1_left.png");
	pacman_.setTexture(LEFT, "../../img/pacman/pacman2_left.png");
	pacman_.setTexture(UP, "../../img/pacman/pacman1_up.png");
	pacman_.setTexture(UP, "../../img/pacman/pacman2_up.png");
	pacman_.setTexture(DOWN, "../../img/pacman/pacman1_down.png");
	pacman_.setTexture(DOWN, "../../img/pacman/pacman2_down.png");

	pacman_.updateMap(&map_);
	pacman_.setDelay(200, 200, 200);
	pacman_.setTileSize(40, 40);
	pacman_.setPosition(40, 40);

	gum_.loadFromFile("../../img/pacman/gum.png");
	fillGum();

	for (size_t i = 0; i < gums_.size(); i++)
		noeuds_.push_back(gums_[i]);

	static std::random_device rd;
	static std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> uniform(0, gums_.size() - 1), uniform2(0, 2);

	data_.set(noeuds_)
	   	.near_value(0) // La valeur qui représente la proximité
	   	.far_value(std::numeric_limits<double>::infinity()) // La valeur qui représente l'éloignement
	   	.combinaison_op(std::plus<double>()) // La fonction pour avoir la distance entre deux noeuds à partir des distances entre un noeud A et un noeud B et entre le noeud B et un noeud C
	   	.shorter_than_op(std::less<double>()); // La fonction qui compare deux distance et dit la quelle est la meilleure

	switch (uniform2(gen))
	{
		case 0:
			fruit_.setTexture("../../img/pacman/apple.png");
			break;
		case 1:
			fruit_.setTexture("../../img/pacman/cherry.png");
			break;
		case 2:
			fruit_.setTexture("../../img/pacman/orange.png");
			break;
	}

	fruit_.applyTexture();

	int n = uniform(gen);

	fruit_.setPos(gums_[n].x * map_.getTileWidth(), gums_[n].y* map_.getTileHeight());
	gums_.erase(gums_.begin() + n);

	
}

void Pacman::computeFrame(const sf::Time & elapsedTime)
{
	if (keyboard_.isKeyPressed(sf::Keyboard::Up))
	{
		pacman_.changeDirection(UP);
	}
	else if (keyboard_.isKeyPressed(sf::Keyboard::Down))
	{
		pacman_.changeDirection(DOWN);
	}
	else if (keyboard_.isKeyPressed(sf::Keyboard::Left))
	{
		pacman_.changeDirection(LEFT);
	}
	else if (keyboard_.isKeyPressed(sf::Keyboard::Right))
	{
		pacman_.changeDirection(RIGHT);
	}

	ia();

	pacman_.update();
	fantomeRouge_.update();

	if (pacman_.isOOB())
	{
		orientation_t dir = pacman_.getDirection();
		if (dir == UP) pacman_.setPosition(pacman_.getPosition().x, 0);
		if (dir == DOWN) pacman_.setPosition(pacman_.getPosition().x, 600 - map_.getTileHeight());
		if (dir == LEFT) pacman_.setPosition(800 - map_.getTileWidth(), pacman_.getPosition().y);
		if (dir == RIGHT) pacman_.setPosition(0, pacman_.getPosition().y);
	}

	collision();
}

void Pacman::drawState() const 
{
	map_.draw(window_);

	for (size_t i = 0; i < gums_.size(); i++)
	{
		sf::Sprite temp;

		temp.setTexture(gum_);
		temp.setPosition(gums_[i].x * map_.getTileWidth(), gums_[i].y * map_.getTileHeight());
		window_.draw(temp);
	}

	if(fruitAlive_)
		window_.draw(fruit_.getSprite());

	window_.draw(fantomeRouge_.getSprite());
	window_.draw(pacman_.getSprite());
}

