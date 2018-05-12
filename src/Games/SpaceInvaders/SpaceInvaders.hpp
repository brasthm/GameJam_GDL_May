#ifndef SPACE_INVADER_HPP
#define SPACE_INVADER_HPP

#include "../Game.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class Entity
{
public:
	explicit Entity() = default;
	explicit Entity(sf::RenderWindow* window, const sf::Texture& texture)
		: window_{ window }
	{
		sprite_.setTexture(texture);
	}

	sf::Sprite getSprite() { return sprite_; }

	void display() const
	{
		window_->draw(sprite_);
	}

	void display(sf::Vector2f pos)
	{
		sprite_.setPosition(pos);
		window_->draw(sprite_);
	}

	void move(const sf::Vector2f & delta)
	{
		sprite_.move(delta.x, delta.y);
	}

	void setPosition(const sf::Vector2f & pos)
	{
		move(pos - sprite_.getPosition());
	}

	void operator=(Entity entity) { sprite_ = entity.sprite_; window_ = entity.window_; }

private:
	//écran
	sf::RenderWindow* window_;
	//sprite
	sf::Sprite sprite_;
};

class Pillar
{
public:
	explicit Pillar() = default;

	float getWidth() { return width_; }
	float getHeight() { return height_; }
	
	void addInvader(Entity invader)
	{
		//largeur
		if (invader.getSprite().getGlobalBounds().width > width_) width_ = invader.getSprite().getGlobalBounds().width;
		//hauteur
		if (pillar_.empty())
		{
			height_ += invader.getSprite().getGlobalBounds().height;
		}
		else
		{
			height_ += 2 * invader.getSprite().getGlobalBounds().height;
		}

		pillar_.push_back(invader);
		invader.setPosition({ 0,height_ + pillar_.back().getSprite().getGlobalBounds().height });
	}

	void setPosition(const sf::Vector2f& pos)
	{
		float y = pos.y;
		float x = pos.x;
		for (auto& invader : pillar_)
		{
			invader.setPosition({ x,y });
			y += 2 * invader.getSprite().getGlobalBounds().height; //espace entre les lignes
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

private:
	std::vector<Entity> pillar_;

	//taille
	float width_ = 0;
	float height_ = 0;
};

class Grid
{
public:
	explicit Grid() = default;

	void addPillar(Pillar pillar)
	{
		if (pillar.getHeight() > height_)height_ = pillar.getHeight();
		if (grid_.empty())
		{
			widht_ += pillar.getWidth();
		}
		else
		{
			widht_ += 2 * pillar.getWidth();
		}
		grid_.push_back(pillar);

	}

	void setPosition(const sf::Vector2f& pos)
	{
		float y = pos.y;
		float x = pos.x;
		for (auto& pillar : grid_)
		{
			pillar.setPosition({ x,y });
			//espace entre les colonnes
				//x += 2 * pillar.getWidth(); //largeur du pillar dédoublée
				x += pillar.getWidth() + 8; //+1 pixel (8 en réalité)
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

private:
	std::vector<Pillar> grid_;

	//taille
	float widht_ = 0;
	float height_ = 0;
};

class SpaceInvaders : public Game
{
public:
	explicit SpaceInvaders(sf::RenderWindow& window);
	void computeFrame(const sf::Time& elapsedTime) override;
	void drawState() const override;

	void manageShip(const sf::Time& elapsedTime);
	void manageGrids(const sf::Time& elapsedTime);

private:
	//constantes 
	float ECRAN_X = 800;
	float ECRAN_Y = 600;

	//varaibles de jeu
	sf::Time age_;

	//textures 
	std::vector<sf::Texture> textures_;

	//ennemis (chaque grid correspond à une sprite de l'animation pour chaque ennemi)
	std::vector<Grid> grids_;
	float gridSpeed_ = 400;
	size_t gridCount_ = 0; //détermine quelle grid afficher

	//vaisseau
	Entity ship_;
	float shipSpeed_ = 400;

	//tir
	Entity shoot_;
	float shootSpeed_ = 500;
	bool shootDoesExist_ = false;
};





#endif // !SPACE_INVADER_HPP
