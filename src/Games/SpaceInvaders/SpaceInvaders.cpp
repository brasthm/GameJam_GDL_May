#include "SpaceInvaders.hpp"

SpaceInvaders::SpaceInvaders(sf::RenderWindow& window) : Game{ window }
{
	textures_.resize(17);

	//vaisseau
	textures_[0].loadFromFile("../../img/SpaceInvaders/ship.png");
	ship_ = Entity(&window, textures_[0]);
	ship_.move({ ECRAN_X / 2, ECRAN_Y - ship_.getSprite().getGlobalBounds().height });

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
	//TODO PG à augmenter x8 depuis le dossier Original
	//textures_[7].loadFromFile("../../img/SpaceInvaders/invaderDownB.png");

	//TEXTURES des protections
	textures_[8].loadFromFile("../../img/SpaceInvaders/prot.png");
	textures_[9].loadFromFile("../../img/SpaceInvaders/prot1.png");
	textures_[10].loadFromFile("../../img/SpaceInvaders/prot2.png");
	textures_[11].loadFromFile("../../img/SpaceInvaders/prot3.png");
	//TODO PG à augmenter x8 depuis le dossier Original
	//textures_[12].loadFromFile("../../img/SpaceInvaders/protHalf.png");
	//textures_[13].loadFromFile("../../img/SpaceInvaders/protHalf1.png");
	//textures_[14].loadFromFile("../../img/SpaceInvaders/protHalf2.png");
	//textures_[15].loadFromFile("../../img/SpaceInvaders/protHalf3.png");

	//TEXTURE du tir
	textures_[16].loadFromFile("../../img/SpaceInvaders/shootA.png");

	//Création du tir


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
		//pillar 8
		gridA.addPillar(pillarA1);
		//pillar 9
		gridA.addPillar(pillarA1);
		//pillar 10
		gridA.addPillar(pillarA1);
		//pillar 11
		gridA.addPillar(pillarA1);
	grids_.push_back(gridA);

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
		//pillar 8
		gridB.addPillar(pillarB1);
		//pillar 9
		gridB.addPillar(pillarB1);
		//pillar 10
		gridB.addPillar(pillarB1);
		//pillar 11
		gridB.addPillar(pillarB1);
	grids_.push_back(gridB);

	for (auto grid : grids_)
		grid.setPosition({ 12 * 8,8 });
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
	if (500 <= (age_.asMilliseconds() % 1000) && (age_.asMilliseconds() % 1000) <= 1000)
	{
		gridCount_++;
		if (gridCount_ >= grids_.size()) gridCount_ = 0;
	}

	grids_[gridCount_].move({ gridSpeed_*elapsedTime.asSeconds(), 0 });

	age_ += elapsedTime;
}


void SpaceInvaders::computeFrame(const sf::Time & elapsedTime)
{
	manageShip(elapsedTime);
	manageGrids(elapsedTime);
}

void SpaceInvaders::drawState() const
{
	auto size = window_.getSize();
	sf::RectangleShape rect(window_.mapPixelToCoords({ (int)size.x, (int)size.y }));
	rect.setFillColor(sf::Color::Black);
	window_.draw(rect);

	//vaisseau
	ship_.display();

	//grid
	grids_[gridCount_].display();
}
