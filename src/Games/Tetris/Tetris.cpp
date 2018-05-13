#include"Tetris.hpp"
#include <random>

void Shape::assign(const sf::Color color, std::string texturestr, char name)
{
	color_ = color;
	name_ = name;
	Tshape_.loadFromFile(texturestr);
	Sshape_.setTexture(Tshape_);
	Sshape_.setColor(color_);
	Sshape_.setPosition(56, 56);
	Height_ = Sshape_.getTextureRect().height;
	width_ = Sshape_.getTextureRect().width;
	pos_.x = 56; pos_.y = 56;
	angle_ = 0;
}

void Shape::updateHW()
{
	int tmp = Height_;
	Height_ = width_;
	width_ = tmp;
}

Tetris::Tetris(sf::RenderTarget& window) : Game(window)
{
	Tvide_.loadFromFile("../../img/tetris/vide.png");
	Svide_.setTexture(Tvide_);

	Tblock_.loadFromFile("../../img/tetris/cube.png");
	Sblock_.setTexture(Tblock_);
	Sblock_.setColor(sf::Color(128, 128, 128, 255));

	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 14; j++)
		{
			Svide_.setPosition(j * 56, i * 56);
			row_.push_back(Svide_);
		}
		grille_.push_back(row_);
		row_.clear();
	}
		

	for (size_t i = 0; i < 10; i++)
	{
		Sblock_.setPosition(0, i * 56);
		grille_[i][0] = Sblock_;
		Sblock_.setPosition(9*56, i * 56);
		grille_[i][9] = Sblock_;
	}
	for (size_t i = 0; i < 14; i++)
	{
		Sblock_.setPosition(i*56, 0);
		grille_[0][i] = Sblock_;
		Sblock_.setPosition(i*56, 9*56);
		grille_[9][i] = Sblock_;
	}
	
	Carrer_.assign(sf::Color(255, 255, 0, 255), "../../img/tetris/carrer.png", 'C');
	
	Barre_.assign(sf::Color(51,153,255,255), "../../img/tetris/barre.png", 'B');
	
	LDroit_.assign(sf::Color(255, 128, 0, 255), "../../img/tetris/LD.png", 'D');
	
	LMirroire_.assign(sf::Color(0, 0, 204, 255), "../../img/tetris/LM.png", 'M');
	
	ZigZagLeft_.assign(sf::Color(0, 204, 0, 255), "../../img/tetris/zigL.png", 'L');
	
	ZigZagRight_.assign(sf::Color(255, 0, 0, 255), "../../img/tetris/zigR.png", 'R');

	T_.assign(sf::Color(153, 0, 153, 255), "../../img/tetris/T.png", 'T');

	posx_ = 0;
	//generer shape alea
	selectShape();
}

void Tetris::moveShape(Shape& shape, const sf::Time& elapsedTime)
{
	sf::Time chuteselect = chute_;
	sf::Keyboard kb;
	sensiprogression_ += elapsedTime;
	chuteprogression_ += elapsedTime;

	if (sensiprogression_ >= Sensi_)
	{
		if (kb.isKeyPressed(sf::Keyboard::Left))
			if (shape.getPos().x > 56)
				shape.move(-56, 0);

		if (kb.isKeyPressed(sf::Keyboard::Right))
			if ((shape.getPos().x + shape.getWidth()) < 9*56)
				shape.move(56, 0);

		if (kb.isKeyPressed(sf::Keyboard::Down))
			chuteselect = chute_rapide;
		else
			chuteselect = chute_;

		if (kb.isKeyPressed(sf::Keyboard::Numpad4))
			rotateLeft(shape);

		if (kb.isKeyPressed(sf::Keyboard::Numpad6))
			rotateRight(shape);

		sensiprogression_ = sf::Time::Zero;
	}

	if (chuteprogression_ >= chuteselect)
	{
		shape.move(0, 56);
		chuteprogression_ = sf::Time::Zero;
	}
}

bool Tetris::colli(Shape& shape) 
{
	int posx = shape.getPos().x;
	int posy = shape.getPos().y;
	int angle = shape.getSprite().getRotation();
	char name = shape.getname();

	posx /= 56; posy /= 56; posy++;

	switch (name)
	{
	case 'C':
		if (angle == 0)
		{
			if (grille_[posx][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx + 1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy+1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx + 1][posy+1].getTexture() != Svide_.getTexture())
				return true;
		}
		break;
	case 'B':
		if (angle == 0)
		{
			if (grille_[posx][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+2][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+3 ][posy].getTexture() != Svide_.getTexture())
				return true;
		}
		else if (angle == 90)
		{
			if (grille_[posx][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx ][posy-1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy-2].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx ][posy-3].getTexture() != Svide_.getTexture())
				return true;
		}
		else if (angle == 180)
		{
			if (grille_[posx][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx - 1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx - 2][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx - 3][posy].getTexture() != Svide_.getTexture())
				return true;
		}
		else
		{
			if (grille_[posx][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy + 1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy + 2].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy + 3].getTexture() != Svide_.getTexture())
				return true;
		}
		break;
	case 'D':
		if (angle == 0)
		{
			if (grille_[posx][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy + 1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+2][posy].getTexture() != Svide_.getTexture())
				return true;
		}
		else if (angle == 90)
		{
			if (grille_[posx][posy ].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy -1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy -2].getTexture() != Svide_.getTexture())
				return true;
		}
		else if (angle == 180)
		{
			if (grille_[posx][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy -1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx-1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy -2].getTexture() != Svide_.getTexture())
				return true;
		}
		else
		{
			if (grille_[posx][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx-1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy + 1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy + 2].getTexture() != Svide_.getTexture())
				return true;
		}
		break;
	case 'M':
		if (angle == 0)
		{
			if (grille_[posx][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+2][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+2][posy +1].getTexture() != Svide_.getTexture())
				return true;
		}
		else if (angle == 90)
		{
			if (grille_[posx][posy].getTexture() != Svide_.getTexture())
				return true;
			if(grille_[posx][posy -1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy-2].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+1][posy -2].getTexture() != Svide_.getTexture())
				return true;
		}
		else if (angle == 180)
		{
			if (grille_[posx][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx-1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx-2][posy ].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx-2][posy -1].getTexture() != Svide_.getTexture())
				return true;
		}
		else
		{
			if (grille_[posx][posy ].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy + 1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy + 2].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx-1][posy + 2].getTexture() != Svide_.getTexture())
				return true;
		}
		break;
	case 'L':
		if (angle == 0)
		{
			if (grille_[posx][posy ].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+1][posy ].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+1][posy + 1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+2][posy +1].getTexture() != Svide_.getTexture())
				return true;
		}
		else if (angle == 90)
		{
			if (grille_[posx][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy -1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+1][posy -1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+1][posy -2].getTexture() != Svide_.getTexture())
				return true;
		}
		else if (angle == 180)
		{
			if (grille_[posx][posy ].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx-1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx-1][posy -1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx-2][posy -1].getTexture() != Svide_.getTexture())
				return true;
		}
		else
		{
			if (grille_[posx][posy ].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy +1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx-1][posy +1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx-1][posy + 2].getTexture() != Svide_.getTexture())
				return true;
		}
		break;
	case 'R':
		if (angle == 0)
		{
			if (grille_[posx][posy+1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+1][posy+1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+2][posy].getTexture() != Svide_.getTexture())
				return true;
		}
		else if (angle == 90)
		{
			if (grille_[posx+1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+1][posy -1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy -1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy-2].getTexture() != Svide_.getTexture())
				return true;
		}
		else if (angle == 180)
		{
			if (grille_[posx][posy - 1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx - 1][posy - 1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx - 1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx - 2][posy].getTexture() != Svide_.getTexture())
				return true;
		}
		else
		{
			if (grille_[posx - 1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx - 1][posy + 1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy + 1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy + 2].getTexture() != Svide_.getTexture())
				return true;
		}
		break;
	case 'T':
		if (angle == 0)
		{
			if (grille_[posx][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+1][posy + 1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+2][posy ].getTexture() != Svide_.getTexture())
				return true;
		}
		else if (angle == 90)
		{
			if (grille_[posx][posy ].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy -1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx+1][posy -1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy -2].getTexture() != Svide_.getTexture())
				return true;
		}
		else if (angle == 180)
		{
			if (grille_[posx][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx -1][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx - 1][posy - 1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx - 2][posy].getTexture() != Svide_.getTexture())
				return true;
		}
		else
		{
			if (grille_[posx][posy].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy + 1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx - 1][posy + 1].getTexture() != Svide_.getTexture())
				return true;
			if (grille_[posx][posy + 2].getTexture() != Svide_.getTexture())
				return true;
		}
		break;
	default:
		return false;
		break;
	}
	return false;
}

void Tetris::updateGid(Shape& shape)
{
	int posx = shape.getPos().x;
	int posy = shape.getPos().y;
	int angle = shape.getSprite().getRotation();
	char name= shape.getname();

	posx /= 56; posy /= 56;
	
	switch (name)
	{
	case 'C' :
		setAt(posx, posy, shape.getSprite());
		setAt(posx+1, posy, shape.getSprite());
		setAt(posx, posy+1, shape.getSprite());
		setAt(posx+1, posy+1, shape.getSprite());
		break;

	case 'B' : 
		if (angle == 0)
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx+1, posy, shape.getSprite());
			setAt(posx+2, posy, shape.getSprite());
			setAt(posx+3, posy, shape.getSprite());
		}
		else if (angle == 90)
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx, posy -1, shape.getSprite());
			setAt(posx, posy -2, shape.getSprite());
			setAt(posx, posy -3, shape.getSprite());
		}
		else if (angle == 180)
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx-1, posy, shape.getSprite());
			setAt(posx-2, posy, shape.getSprite());
			setAt(posx-3, posy, shape.getSprite());
		}
		else
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx , posy+1, shape.getSprite());
			setAt(posx , posy+2, shape.getSprite());
			setAt(posx , posy+3, shape.getSprite());
		}
		break;
	case 'D' :
		if (angle == 0)
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx, posy+1, shape.getSprite());
			setAt(posx+1, posy, shape.getSprite());
			setAt(posx+2, posy, shape.getSprite());
		}
		else if (angle == 90)
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx+1, posy, shape.getSprite());
			setAt(posx, posy -1, shape.getSprite());
			setAt(posx, posy -2, shape.getSprite());
		}
		else if (angle == 180)
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx, posy - 1, shape.getSprite());
			setAt(posx - 1, posy, shape.getSprite());
			setAt(posx - 2, posy, shape.getSprite());
		}
		else
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx -1, posy, shape.getSprite());
			setAt(posx, posy +1, shape.getSprite());
			setAt(posx, posy+2, shape.getSprite());
		}
		break;
	case 'M' :
		if (angle == 0)
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx + 1, posy, shape.getSprite());
			setAt(posx+2, posy , shape.getSprite());
			setAt(posx+2, posy+1, shape.getSprite());
		}
		else if (angle == 90)
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx , posy-1, shape.getSprite());
			setAt(posx, posy-2, shape.getSprite());
			setAt(posx + 1, posy -2, shape.getSprite());
		}
		else if (angle == 180)
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx - 1, posy, shape.getSprite());
			setAt(posx - 2, posy, shape.getSprite());
			setAt(posx - 2, posy - 1, shape.getSprite());
		}
		else
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx, posy + 1, shape.getSprite());
			setAt(posx, posy + 2, shape.getSprite());
			setAt(posx - 1, posy + 2, shape.getSprite());
		}
		break;
	case 'L' :
		if (angle == 0)
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx+1, posy , shape.getSprite());
			setAt(posx+1, posy + 1, shape.getSprite());
			setAt(posx + 2, posy + 1, shape.getSprite());
		}
		else if (angle == 90)
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx, posy - 1, shape.getSprite());
			setAt(posx+1, posy - 1, shape.getSprite());
			setAt(posx + 1, posy - 2, shape.getSprite());
		}
		else if (angle == 180)
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx - 1, posy, shape.getSprite());
			setAt(posx - 1, posy - 1, shape.getSprite());
			setAt(posx - 2, posy - 1, shape.getSprite());
		}
		else
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx, posy + 1, shape.getSprite());
			setAt(posx - 1, posy + 1, shape.getSprite());
			setAt(posx - 1, posy + 2, shape.getSprite());
		}
		break;
	case 'R' :
		if (angle == 0)
		{
			setAt(posx, posy+1, shape.getSprite());
			setAt(posx+1, posy+1 , shape.getSprite());
			setAt(posx+1, posy , shape.getSprite());
			setAt(posx + 2, posy , shape.getSprite());
		}
		else if (angle == 90)
		{
			setAt(posx+1, posy, shape.getSprite());
			setAt(posx+1, posy - 1, shape.getSprite());
			setAt(posx, posy - 1, shape.getSprite());
			setAt(posx, posy - 2, shape.getSprite());
		}
		else if (angle == 180)
		{
			setAt(posx, posy - 1, shape.getSprite());
			setAt(posx - 1, posy - 1, shape.getSprite());
			setAt(posx - 1, posy, shape.getSprite());
			setAt(posx - 2, posy, shape.getSprite());
		}
		else
		{
			setAt(posx - 1, posy, shape.getSprite());
			setAt(posx - 1, posy + 1, shape.getSprite());
			setAt(posx, posy + 1, shape.getSprite());
			setAt(posx, posy + 2, shape.getSprite());
		}
		break;
	case 'T':
		if (angle == 0)
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx+1, posy, shape.getSprite());
			setAt(posx+1, posy+1, shape.getSprite());
			setAt(posx + 2, posy , shape.getSprite());
		}
		else if (angle == 90)
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx, posy - 1, shape.getSprite());
			setAt(posx+1, posy - 1, shape.getSprite());
			setAt(posx , posy - 2, shape.getSprite());
		}
		else if (angle == 180)
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx - 1, posy, shape.getSprite());
			setAt(posx -1, posy - 1, shape.getSprite());
			setAt(posx - 2, posy, shape.getSprite());
		}
		else
		{
			setAt(posx, posy, shape.getSprite());
			setAt(posx, posy + 1, shape.getSprite());
			setAt(posx - 1, posy + 1, shape.getSprite());
			setAt(posx, posy + 2, shape.getSprite());
		}
		break;
	default:
		break;
	}
	
}

void Tetris::rotateLeft(Shape& shape) 
{
	shape.AddAngle(90);
}

void Tetris::rotateRight(Shape& shape)
{
	shape.AddAngle(-90);
}

void Tetris::selectShape()
{
	int spawn;
	static std::random_device rd;
	static std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> uniform(0, 6);
	spawn = uniform(gen);
	switch (spawn)
	{
	case 0 :
		shapecourrante_ = Carrer_;
		break;
	case 1:
		shapecourrante_ = Barre_;
		break;
	case 2:
		shapecourrante_ = LDroit_;
		break;
	case 3:
		shapecourrante_ = LMirroire_;
		break;
	case 4:
		shapecourrante_ = ZigZagLeft_;
		break;
	case 5:
		shapecourrante_ = ZigZagRight_;
		break;
	case 6:
		shapecourrante_ = T_;
		break;
	default:
		break;
	}
}

int Tetris::isRow() 
{
	int nbcube = 0;
	for (size_t l = 1; l <9; l++)
	{
		for (size_t c = 1; c <9; c++)
		{
			if (grille_[c][l].getTexture() != Svide_.getTexture())
				nbcube++;
		}
			if (nbcube == 8)
				return l;
			else
				nbcube = 0;
	}
	return -1;

}

void Tetris::destroyRow(int row)
{
	for (size_t c = 1; c < 9; c++)
		grille_[c][row] = Svide_;
}

bool Tetris::computeFrame(const sf::Time& elapsedTime, int& score)
{
	if (!colli(shapecourrante_))
		  moveShape(shapecourrante_, elapsedTime);
	else
	{
		if (shapecourrante_.getPos().y == 56)
			return false;
		else
		{
			updateGid(shapecourrante_);
			selectShape();
			shapecourrante_.setPos(56, 56);
		}
	}

	int row = isRow();
	if (row != -1)
		destroyRow(row);
	
	return true;
}

void Tetris::drawState(sf::Sprite &countdown)const
{
	auto size = window_.getSize();
	sf::RectangleShape rect(window_.mapPixelToCoords({ (int)size.x, (int)size.y }));
	rect.setFillColor(sf::Color::Black);
	window_.draw(rect);

	for (size_t i = 0; i < 10; i++)
		for (size_t j = 0; j < 14; j++)
			if(grille_[i][j].getTexture() != Svide_.getTexture())
				window_.draw(grille_[i][j]);

	window_.draw(shapecourrante_.getSprite());

}
