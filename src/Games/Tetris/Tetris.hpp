#ifndef TETRIS_HPP
#define TETRIS_HPP

#include "../Game.hpp"
#include "../../Sprite.h"
#include <vector>
#include <string>

class Shape
{
private:
	sf::Color color_;
	sf::Sprite Sshape_;
	sf::Texture Tshape_;
	int Height_, width_;
	char name_;
	sf::Vector2i pos_;
	int angle_;

public:
	Shape() {};
	void assign(const sf::Color color, std::string texturestr, char name);
	const sf::Color& getColor() const { return color_; }
	sf::Sprite& getSprite() { return Sshape_; }
	const sf::Sprite& getSprite() const { return Sshape_; }
	void setPos(float x, float y) { Sshape_.setPosition(x, y); pos_.x = x; pos_.y = y; }
	const sf::Vector2i& getPos()const { return pos_; }
	void move(float x, float y) { Sshape_.move(x, y); pos_.x += x; pos_.y += y; }
	void updateHW();
	const int& getHeight() const { return Height_; }
	const int& getWidth() const { return width_; }
	const char& getname()const { return name_; }
	void AddAngle(int ang) { angle_ += ang; }
	const int& getAngle()const { return angle_; }
};


class Tetris : public Game
{
private:
	sf::Texture Tblock_;
	sf::Sprite Sblock_;
	sf::Texture Tvide_;
	sf::Sprite Svide_;
	std::vector<std::vector<sf::Sprite>> grille_;
	std::vector<sf::Sprite> row_;
	Shape Carrer_;
	Shape Barre_;
	Shape LDroit_;
	Shape LMirroire_;
	Shape ZigZagLeft_;
	Shape ZigZagRight_;
	Shape T_;
	Shape shapecourrante_;
	int posx_;
	sf::Time Sensi_ = sf::milliseconds(100);
	sf::Time sensiprogression_;
	sf::Time chute_ = sf::milliseconds(600);
	sf::Time chute_rapide = sf::milliseconds(100);
	sf::Time chuteprogression_;

public:
	explicit Tetris(sf::RenderWindow& window);
	void computeFrame(const sf::Time& elapsedTime) override;
	void drawState() const override;
	void setAt(int x, int y, const sf::Sprite& elem) { grille_[x][y] = elem; }
	void moveShape(Shape&, const sf::Time& elapsedTime);
	void rotateLeft(Shape&);
	void rotateRight(Shape&);
	void selectShape();
	void updateGid(Shape&);
	bool colli(Shape&);
	int isRow();
	void destroyRow(int);
};

#endif // !TETRIS_HPP