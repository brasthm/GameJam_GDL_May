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
		sf::Sprite sprite;
		sprite.setTexture(texture);
		sprites_.push_back(sprite);
	}

	void addSprite(const sf::Texture& texture)
	{
		sf::Sprite sprite;
		sprite.setTexture(texture);
		sprites_.push_back(sprite);
	}

	void operator=(Entity entity) { sprites_ = entity.sprites_; window_ = entity.window_; }

	std::vector<sf::Sprite> getSprites() const { return sprites_; }
	Team getTeam() const { return team_; }
	sf::FloatRect getGlobalBounds() const { return sprites_.front().getGlobalBounds(); }
	sf::Vector2f getPosition() const { return sprites_.front().getPosition(); }

	bool isAlive() { return alive_; }
	void setTeam(Team team) { team_ = team; }
	void setAlive(bool alive) { alive_ = alive; }

	void rotate(float angle)
	{
		for (auto& sprite : sprites_)
			sprite.rotate(angle);
	}
	
	void display() const
	{
		if (alive_) window_->draw(sprites_.front());
	}

	void display(size_t spriteCount) const
	{
		if (alive_) window_->draw(sprites_[spriteCount]);
	}

	//void display(sf::Vector2f pos)
	//{
	//	sprite_.setPosition(pos);
	//	window_->draw(sprite_);
	//}

	virtual void move(const sf::Vector2f & delta)
	{
		for(auto& sprite : sprites_)
			sprite.move(delta.x, delta.y);
	}

	virtual void setPosition(const sf::Vector2f & pos)
	{
		for (auto& sprite : sprites_)
			sprite.move(pos - sprite.getPosition());
	}

	virtual bool collision(sf::Sprite& sprite)
	{
		if (alive_ && sprite.getGlobalBounds().intersects(sprites_.front().getGlobalBounds()))
		{
			alive_ = false;
			return true;
		}
		else return false;
	}

private:
	//écran
	sf::RenderTarget* window_;
	//sprites
	std::vector<sf::Sprite> sprites_;
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
		if (invader.getGlobalBounds().width > width_) width_ = invader.getGlobalBounds().width;
		//hauteur
		height_ += invader.getGlobalBounds().height;
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
			y += spaceBetweenLines_ + invader.getGlobalBounds().height;
		}
	}

	void move(const sf::Vector2f & delta)
	{
		for (auto& invader : pillar_)
			invader.move(delta);
	}

	void display(size_t spriteCount) const
	{
		for (auto& invader : pillar_)
			invader.display(spriteCount);
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
			if (invader.getPosition().y > pos.y) 
			{ 
				pos.y = invader.getPosition().y + invader.getGlobalBounds().height;
				pos.x = invader.getPosition().x + invader.getGlobalBounds().width / 2.f;
			}
		}

		return pos;
	}

	sf::Vector2f getPosition() const { return pillar_.front().getPosition(); }

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

	void display(size_t spriteCount) const
	{
		for (auto& pillar : grid_)
			pillar.display(spriteCount);
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
	void setSpriteCount(size_t count) { spriteCount_ = count; }

	std::vector<Pillar> getGrid() const { return grid_; }
	Direction getDirection() const { return direction_; }
	float getWidth() const { return width_; }
	float getHeight() const { return height_; }
	sf::Vector2f getPosition() const { return grid_.front().getPosition(); }
	size_t getSpriteCount() const { return spriteCount_; }

	//HACK
	void setWidth(float f) { width_ = f; }

private:
	std::vector<Pillar> grid_;
	float spaceBetweenColumns_ = 0; //espace entre les colonnes

	size_t spriteCount_ = 0; //détermine quelle sprite afficher

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

	//variables de jeu
	sf::Time age_;
	int SI_score_ = 0;

	//textures 
	std::vector<sf::Texture> textures_;

	//Grid d'ennemi
	std::vector<Grid> grids_;
	float gridSpeed_ = 100;
		//détermine le temps de clignotement
	const sf::Time gridBlinkTime_ = sf::milliseconds(750);
	sf::Time gridBlinkAge_;
		//détermine la limite de déplacement vers le bas
	float downLimit_ = 0;
		//détermine le sens de dernière translation (true : droite)
	bool lastTranslation_ = true;

	//vaisseau
	Entity ship_;
	float shipSpeed_ = 800;

	//tirs
	float shootSpeed_ = 1000;
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
