#ifndef SPACE_INVADER_HPP
#define SPACE_INVADER_HPP

#include "../Game.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <vector>

constexpr float SPACE_BETWEEN_LINES = 8 * 3;

enum Team {SI_FRIEND, SI_ENNEMY, SI_NEUTRAL};

class Entity
{
public:
	explicit Entity() = default;
	explicit Entity(sf::RenderTarget* window, const sf::Texture& texture)
		: window_{ window }
	{
		sprite_.setTexture(texture);
	}
	void operator=(Entity entity) { sprite_ = entity.sprite_; window_ = entity.window_; }

	sf::Sprite getSprite() { return sprite_; }
	Team getTeam() { return team_; }
	bool isAlive() { return alive_; }
	void setTeam(Team team) { team_ = team; }
	void setAlive(bool alive) { alive_ = alive; }

	void display() const
	{
		if (alive_) window_->draw(sprite_);
	}

	void display(sf::Vector2f pos)
	{
		sprite_.setPosition(pos);
		window_->draw(sprite_);
	}

	virtual void move(const sf::Vector2f & delta)
	{
		sprite_.move(delta.x, delta.y);
	}

	virtual void setPosition(const sf::Vector2f & pos)
	{
		move(pos - sprite_.getPosition());
	}

	virtual bool collision(sf::Sprite& sprite)
	{
		if (alive_ && sprite.getGlobalBounds().intersects(sprite_.getGlobalBounds()))
		{
			alive_ = false;
			return true;
		}
		else return false;
	}

private:
	//�cran
	sf::RenderTarget* window_;
	//sprite
	sf::Sprite sprite_;
	//jeu
	bool alive_ = true;
	Team team_ = SI_NEUTRAL;
};

//TODO abri
class Shelter : public Entity
{
public:
	void addProtFull(const sf::Texture& texture)
	{
		sf::Sprite sprite;
		sprite.setTexture(texture);
		protsFull_.push_back(sprite);
	}
	void addProtHalf(const sf::Texture& texture)
	{
		sf::Sprite sprite;
		sprite.setTexture(texture);
		protsHalf_.push_back(sprite);
	}
private:
	std::vector<sf::Sprite> protsFull_;
	std::vector<sf::Sprite> protsHalf_;
};

class Pillar
{
public:
	explicit Pillar() = default;

	float getWidth() { return width_; }
	float getHeight() { return height_; }

	//HACK pas beau
	std::vector<Entity> getPillar() { return pillar_; }


	void setTeam(Team team)
	{
		for (auto& invader : pillar_)
			invader.setTeam(team);
	}

	void setAlive(bool alive)
	{
		for (auto& invader : pillar_)
			invader.setAlive(alive);
	}
	
	void addInvader(Entity invader)
	{
		//largeur
		if (invader.getSprite().getGlobalBounds().width > width_) width_ = invader.getSprite().getGlobalBounds().width;
		//hauteur
		height_ += invader.getSprite().getGlobalBounds().height;
		if (!pillar_.empty()) height_ += spaceBetweenLines_;

		pillar_.push_back(invader);
		setPosition({ 0,0 });
	}

	void setPosition(const sf::Vector2f& pos)
	{
		float y = pos.y;
		float x = pos.x;
		for (auto& invader : pillar_)
		{
			invader.setPosition({ x,y });
			y += spaceBetweenLines_ + invader.getSprite().getGlobalBounds().height;
		}
	}

	void move(const sf::Vector2f & delta)
	{
		for (auto& invader : pillar_)
			invader.move(delta);
	}

	void display() const
	{
		for (auto& invader : pillar_)
			invader.display();
	}

	bool collision(sf::Sprite sprite)
	{
		for (auto& invader : pillar_)
		{
			if (invader.collision(sprite))
				return true;
		}
		return false;
	}

	sf::Vector2f getPillarBottomPos()
	{
		sf::Vector2f pos = { 0,0 };
		for (auto& invader : pillar_)
		{
			if (invader.getSprite().getPosition().y > pos.y) 
			{ 
				pos.y = invader.getSprite().getPosition().y + invader.getSprite().getGlobalBounds().height;
				pos.x = invader.getSprite().getPosition().x + invader.getSprite().getGlobalBounds().width / 2.f;
			}
		}

		return pos;
	}

	sf::Vector2f getPosition() { return pillar_.front().getSprite().getPosition(); }

	void setSpaceBetweenLine(float f) { spaceBetweenLines_ = f; }

private:
	std::vector<Entity> pillar_;
	float spaceBetweenLines_ = SPACE_BETWEEN_LINES; //espace entre les lignes

	//taille
	float width_ = 0;
	float height_ = 0;
};

enum Direction { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };

class Grid
{
public:
	explicit Grid() = default;

	void addPillar(Pillar pillar)
	{
		grid_.push_back(pillar);
		setPosition({ 0,0 });

		//hauteur
		if (pillar.getHeight() > height_) height_ = pillar.getHeight();
		//largeur
		width_ += spaceBetweenColumns_;
	}

	void setTeam(Team team)
	{
		for (auto& pillar : grid_)
			pillar.setTeam(team);
	}

	void setAlive(bool alive)
	{
		for (auto& pillar : grid_)
			pillar.setAlive(alive);
	}

	void setPosition(const sf::Vector2f& pos)
	{
		float y = pos.y;
		float x = pos.x;
		for (auto& pillar : grid_)
		{
			pillar.setPosition({ x,y });
				x += pillar.getWidth() + spaceBetweenColumns_;
		}
	}

	void move(const sf::Vector2f & delta)
	{
		for (auto& pillar : grid_)
			pillar.move(delta);
	}

	void display() const
	{
		for (auto& pillar : grid_)
			pillar.display();
	}

	bool collision(sf::Sprite sprite)
	{
		for (auto& pillar : grid_)
		{
			if (pillar.collision(sprite))
				return true;
		}
		return false;
	}

	void setSpaceBetweenColumns(float f) { spaceBetweenColumns_ = f; }
	void setDirection(Direction direction) { direction_ = direction; }

	std::vector<Pillar> getGrid() { return grid_; }
	Direction getDirection() { return direction_; }
	float getWidth() { return width_; }
	float getHeight() { return height_; }
	sf::Vector2f getPosition() { return grid_.front().getPosition(); }

	//HACK
	void setWidth(float f) { width_ = f; }

private:
	std::vector<Pillar> grid_;
	float spaceBetweenColumns_ = 0; //espace entre les colonnes

	//taille
	float width_ = 0;
	float height_ = 0;

	//direction
	Direction direction_ = DIR_RIGHT;
};

class SpaceInvaders : public Game
{
public:
	explicit SpaceInvaders(sf::RenderTarget& window, DJ& dj);
	bool computeFrame(const sf::Time& elapsedTime, int& score) override;
	void drawState(sf::Sprite &countdown) const override;

	void manageShip(const sf::Time& elapsedTime);
	void manageGrids(const sf::Time& elapsedTime);
	void manageShoot(const sf::Time& elapsedTime);

private:
	//constantes 
	const float ECRAN_X = 800;
	const float ECRAN_Y = 600;
	float borderSpace_ = 0; //espace sur le côté

	//varaibles de jeu
	sf::Time age_;

	//textures 
	std::vector<sf::Texture> textures_;

	//Grids (chaque grid correspond à une sprite de l'animation pour chaque ennemi)
	std::vector<Grid> grids_;
	const float gridSpeed_ = 75;
	size_t gridCount_ = 0; //détermine quelle grid afficher
		//détermine le temps de clignotement
	const sf::Time gridBlinkTime_ = sf::milliseconds(750);
	sf::Time gridBlinkAge_;
		//détermine la limite de déplacement vers le bas
	float downLimit_ = 0;
		//détermine le sens de dernière translation (true : droite)
	bool lastTranslation_ = true;

	//vaisseau
	Entity ship_;
	float shipSpeed_ = 400;

	//tirs
	float shootSpeed_ = 500;
		//détermine le temps de clignotement
	const sf::Time shootBlinkTime_ = sf::milliseconds(1000); //TODO marche pas
	sf::Time shootBlinkAge_;
		//conteneur de tirs
	std::vector<Entity> shoots_;

	//tir joueur
	Entity shoot_; 
	bool playerShootDoesExist_ = false;
};





#endif // !SPACE_INVADER_HPP
