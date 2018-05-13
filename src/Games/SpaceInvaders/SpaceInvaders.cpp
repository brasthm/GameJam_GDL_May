#include "SpaceInvaders.hpp"
#include <cmath>
#include <random>

SpaceInvaders::SpaceInvaders(sf::RenderTarget& window) : Game{ window }
{
	textures_.resize(17);

	//vaisseau
	textures_[0].loadFromFile("../../img/SpaceInvaders/ship.png");
	ship_ = Entity(&window, textures_[0]);
	ship_.move({ ECRAN_X / 2, ECRAN_Y - ship_.getSprite().getGlobalBounds().height });//initial pos
	ship_.setTeam(SI_FRIEND);
	borderSpace_ = std::ceil(ship_.getSprite().getGlobalBounds().width / 2.f);

	//TEXTURES des ennemis
	//invader haut
	textures_[1].loadFromFile("../../img/SpaceInvaders/invaderUpA.png");
	textures_[2].loadFromFile("../../img/SpaceInvaders/invaderUpB.png");
	//invader mileu
	textures_[3].loadFromFile("../../img/SpaceInvaders/invaderMiddleA.png");
	textures_[4].loadFromFile("../../img/SpaceInvaders/invaderMiddleB.png");
	//invadeur bas
	textures_[5].loadFromFile("../../img/SpaceInvaders/invaderDownA.png");
	textures_[6].loadFromFile("../../img/SpaceInvaders/invaderDownB.png");
	//invader Boss
	//TODO PG � augmenter x8 depuis le dossier Original
	//textures_[7].loadFromFile("../../img/SpaceInvaders/invaderDownB.png");

	//TEXTURES des protections
	textures_[8].loadFromFile("../../img/SpaceInvaders/prot.png");
	textures_[9].loadFromFile("../../img/SpaceInvaders/prot1.png");
	textures_[10].loadFromFile("../../img/SpaceInvaders/prot2.png");
	textures_[11].loadFromFile("../../img/SpaceInvaders/prot3.png");
	//TODO PG � augmenter x8 depuis le dossier Original
	//textures_[12].loadFromFile("../../img/SpaceInvaders/protHalf.png");
	//textures_[13].loadFromFile("../../img/SpaceInvaders/protHalf1.png");
	//textures_[14].loadFromFile("../../img/SpaceInvaders/protHalf2.png");
	//textures_[15].loadFromFile("../../img/SpaceInvaders/protHalf3.png");

	//TEXTURE du tir
	textures_[16].loadFromFile("../../img/SpaceInvaders/shootA.png");

	//Création du tir
	shoot_ = Entity(&window, textures_[16]);

	//Création de la Grid A
	Grid gridA;
		//pillar 1
		Pillar pillarA1;
		Entity entityA11(&window, textures_[1]);
		Entity entityA12(&window, textures_[3]);
		Entity entityA13(&window, textures_[3]);
		Entity entityA14(&window, textures_[5]);
		Entity entityA15(&window, textures_[5]);
		pillarA1.addInvader(entityA11);
		pillarA1.addInvader(entityA12);
		pillarA1.addInvader(entityA13);
		pillarA1.addInvader(entityA14);
		pillarA1.addInvader(entityA15);
		gridA.addPillar(pillarA1);
		//pillar 2
		gridA.addPillar(pillarA1);
		//pillar 3
		gridA.addPillar(pillarA1);
		//pillar 4
		gridA.addPillar(pillarA1);
		//pillar 5
		gridA.addPillar(pillarA1);
		//pillar 6 
		gridA.addPillar(pillarA1);
		//pillar 7
		gridA.addPillar(pillarA1);
		////pillar 8
		//gridA.addPillar(pillarA1);
		////pillar 9
		//gridA.addPillar(pillarA1);
		//pillar 10
		//gridA.addPillar(pillarA1);
		//pillar 11
		//gridA.addPillar(pillarA1);
	grids_.push_back(gridA);

	downLimit_ = pillarA1.getPillarBottomPos().y + SPACE_BETWEEN_LINES;

	//Création de la Grid B
	Grid gridB;
		//pillar 1
		Pillar pillarB1;
		Entity entityB11(&window, textures_[2]);
		Entity entityB12(&window, textures_[4]);
		Entity entityB13(&window, textures_[4]);
		Entity entityB14(&window, textures_[6]);
		Entity entityB15(&window, textures_[6]);
		pillarB1.addInvader(entityB11);
		pillarB1.addInvader(entityB12);
		pillarB1.addInvader(entityB13);
		pillarB1.addInvader(entityB14);
		pillarB1.addInvader(entityB15);
		gridB.addPillar(pillarB1);
		//pillar 2
		gridB.addPillar(pillarB1);
		//pillar 3
		gridB.addPillar(pillarB1);
		//pillar 4
		gridB.addPillar(pillarB1);
		//pillar 5
		gridB.addPillar(pillarB1);
		//pillar 6 
		gridB.addPillar(pillarB1);
		//pillar 7
		gridB.addPillar(pillarB1);
		////pillar 8
		//gridB.addPillar(pillarB1);
		////pillar 9
		//gridB.addPillar(pillarB1);
		//pillar 10
		//gridB.addPillar(pillarB1);
		//pillar 11
		//gridB.addPillar(pillarB1);
	grids_.push_back(gridB);

	for (auto& grid : grids_)
	{
		float gridWidth = 0;
		for (auto& pillar : grid.getGrid())
		{
			gridWidth += pillar.getWidth();
		}
		grid.setSpaceBetweenColumns((ECRAN_X - 2 * borderSpace_ - gridWidth) / grid.getGrid().size());
		grid.setWidth(ECRAN_X - 2 * borderSpace_ - (ECRAN_X - 2 * borderSpace_ - gridWidth) / grid.getGrid().size());
		grid.setPosition({ 0,0 });
		grid.setTeam(SI_ENNEMY);
	}

}

void SpaceInvaders::manageShip(const sf::Time& elapsedTime)
{
	sf::Vector2f posbefore = ship_.getSprite().getPosition();
	sf::Keyboard keyboard;

	float deltaX = 0;

	if (keyboard.isKeyPressed(sf::Keyboard::Left))
		deltaX = -shipSpeed_ * elapsedTime.asSeconds();
	else if (keyboard.isKeyPressed(sf::Keyboard::Right))
		deltaX = shipSpeed_ * elapsedTime.asSeconds();

	float taille = ship_.getSprite().getGlobalBounds().width;
	float pos = ship_.getSprite().getPosition().x;
	if ((pos + taille + deltaX) > ECRAN_X) deltaX = ECRAN_X - pos - taille;
	else if (pos + deltaX < 0) deltaX = -pos;
	ship_.move({ deltaX, 0 });

}

void SpaceInvaders::manageGrids(const sf::Time & elapsedTime)
{
	if (gridBlinkAge_ >= gridBlinkTime_)
	{
		gridCount_++;
		if (gridCount_ >= grids_.size()) gridCount_ = 0;
		gridBlinkAge_ = sf::Time::Zero;
	}
	gridBlinkAge_ += elapsedTime;

	//mouvement : création du delta de mouvement
	sf::Vector2f delta = { gridSpeed_*elapsedTime.asSeconds()*(float)(grids_[0].getDirection() == DIR_RIGHT) - gridSpeed_ * elapsedTime.asSeconds()*(float)(grids_[0].getDirection() == DIR_LEFT)
						  ,gridSpeed_*elapsedTime.asSeconds()*(float)(grids_[0].getDirection() == DIR_DOWN ) - gridSpeed_ * elapsedTime.asSeconds()*(float)(grids_[0].getDirection() == DIR_UP  )};
	sf::Vector2f taille = { grids_[0].getWidth(),grids_[0].getHeight() };
	sf::Vector2f pos = grids_[0].getPosition();

	if ((pos.x + taille.x + delta.x) > ECRAN_X) //trop à droite
	{
		delta.x = ECRAN_X - pos.x - taille.x;
		grids_[0].setDirection(DIR_DOWN);
	}
	else if (pos.x + delta.x < 0) //trop à gauche
	{
		delta.x = -pos.x;
		grids_[0].setDirection(DIR_DOWN);
	}

	const float MGK = 60;
		//la magick constant correspond à la taille entre le bas de l'écran et le haut des abris (vaisseau + abri + espace entre les 2)
		// 8*8 + 16*8 = 192

	if(downLimit_ > (ECRAN_Y - MGK)) downLimit_ = (ECRAN_Y - MGK);

	if ((pos.y + taille.y + delta.y) >= downLimit_ ) //trop en bas
	{
		delta.y = downLimit_ - pos.y - taille.y - 1;
		if (lastTranslation_) //si avant on allait à droite
		{
			grids_[0].setDirection(DIR_LEFT);
			lastTranslation_ = false;
		}
		else //si avant on allait à gauche
		{
			grids_[0].setDirection(DIR_RIGHT);
			lastTranslation_ = true;
		}

		downLimit_ += SPACE_BETWEEN_LINES/2.f;
	}
	else if	(pos.y + delta.y < 0) //trop en haut
	{
		delta.y = -pos.y;
		grids_[0].setDirection(DIR_DOWN);
	}

	for (auto& grid : grids_) //màj des positions
	{
		grid.move(delta);
	}

}

void SpaceInvaders::manageShoot(const sf::Time & elapsedTime)
{
	if (shootBlinkAge_ >= shootBlinkTime_)
	{
		for (auto& shoot : shoots_)
			shoot.getSprite().rotate(180);
		shootBlinkAge_ = sf::Time::Zero;
	}
	shootBlinkAge_ += elapsedTime;

	//tir ami
	sf::Keyboard keyboard;
	if (!playerShootDoesExist_ && (keyboard.isKeyPressed(sf::Keyboard::Space) || keyboard.isKeyPressed(sf::Keyboard::Up) || keyboard.isKeyPressed(sf::Keyboard::Down)))
	{
		playerShootDoesExist_ = true;
		shoots_.push_back(shoot_);
		float x = ship_.getSprite().getPosition().x + ship_.getSprite().getGlobalBounds().width / 2.f;
		float y = ship_.getSprite().getPosition().y;
		shoots_.back().setPosition({ ship_.getSprite().getPosition().x + ship_.getSprite().getGlobalBounds().width / 2.f, ship_.getSprite().getPosition().y });
		shoots_.back().setTeam(SI_FRIEND);
	}

	//TODO tir ennemi
	static std::random_device rd;
	static std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> unif_int(0, grids_[0].getGrid().size());
	std::uniform_real_distribution<float> colldown(500, 1500);



	//Gestion des tirs
	for (auto& shoot : shoots_)
	{
		if (shoot.getTeam() == SI_FRIEND)
		{
			shoot.move({ 0,-shootSpeed_ * elapsedTime.asSeconds() });
			if (grids_[0].collision(shoot.getSprite()))
			{
				shoot.setAlive(false);
				playerShootDoesExist_ = false;
			}
			if (shoot.getSprite().getPosition().y < 0)
			{
				shoot.setAlive(false);
				playerShootDoesExist_ = false;
			}
		}
	}

	//Destruction tirs morts
	for (auto it = shoots_.begin(); it != shoots_.end();)
	{
		if (!it->isAlive())
		{
			it = shoots_.erase(it);
		}
		else
		{
			it++;
		}
	}
	
}


bool SpaceInvaders::computeFrame(const sf::Time & elapsedTime, int& score)
{
	manageShip(elapsedTime);
	manageGrids(elapsedTime);
	manageShoot(elapsedTime);
	return true;
	age_ += elapsedTime;
}

void SpaceInvaders::drawState(sf::Sprite &countdown) const
{
	auto size = window_.getSize();
	sf::RectangleShape rect(window_.mapPixelToCoords({ (int)size.x, (int)size.y }));
	rect.setFillColor(sf::Color::Black);
	window_.draw(rect);

	//vaisseau
	ship_.display();

	//grid
	grids_[gridCount_].display();

	//tirs
	for (auto shoot : shoots_)
		shoot.display();

	//Debug
	//sf::RectangleShape line(sf::Vector2f(150, 2));
	//line.setFillColor(sf::Color::Red);
	//line.setPosition({ 0,downLimit_ });
	//window_.draw(line);
}
