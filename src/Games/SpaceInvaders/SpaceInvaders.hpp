#ifndef SPACE_INVADER_HPP
#define SPACE_INVADER_HPP

#include "../Game.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

constexpr float SPACE_BETWEEN_LINES = 8 * 3;

enum Team { SI_FRIEND, SI_ENNEMY, SI_NEUTRAL };

class Entity
{
public:
    explicit Entity() = default;
    explicit Entity(sf::RenderTarget* window, const sf::Texture& texture);

    void addSprite(const sf::Texture& texture);

    std::vector<sf::Sprite> getSprites() const { return sprites_; }
    Team                    getTeam() const { return team_; }
    sf::FloatRect           getGlobalBounds() const { return sprites_.front().getGlobalBounds(); }
    sf::Vector2f            getPosition() const { return sprites_.front().getPosition(); }

    bool  isAlive() const { return alive_; }
    void  setTeam(Team   team) { team_   = team; }
    void  setAlive(bool  alive) { alive_ = alive; }
    void  setSpeed(float speed) { speed_ = speed; }
    float speed() const { return speed_; }

    void centerSpriteOrigin();

    void rotate(float angle);

    void display() const;

    void display(size_t spriteCount) const;

    virtual void move(const sf::Vector2f& delta);

    virtual void setPosition(const sf::Vector2f& pos);

    virtual bool collision(sf::Sprite const& sprite);

    bool collision(Entity const& entity);

private:
    //écran
    sf::RenderTarget* window_;
    //sprites
    std::vector<sf::Sprite> sprites_;
    //jeu
    bool  alive_ = true;
    Team  team_  = SI_NEUTRAL;
    float speed_ = 1000;
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

    //HACK pas beau
    std::vector<Entity> getPillar() const { return pillar_; }

    void setTeam(Team team);

    bool isAlive() const;

    void setAlive(bool alive);

    void addInvader(const Entity& invader);

    void setPosition(const sf::Vector2f& pos);

    void move(const sf::Vector2f& delta);

    void display(size_t spriteCount) const;

    bool collision(sf::Sprite const& sprite);

    float getWidth() const { return width_; }

    float getHeight() const;

    sf::Vector2f getPillarBottomPos() const;

    sf::Vector2f getPosition() const { return pillar_.front().getPosition(); }

    void setSpaceBetweenLine(float f) { spaceBetweenLines_ = f; }

private:
    std::vector<Entity> pillar_;
    float               spaceBetweenLines_ = SPACE_BETWEEN_LINES; //espace entre les lignes

    //taille
    float width_ = 0;
    float height_ = 0;
};

enum Direction { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };

class Grid
{
public:
    explicit Grid() = default;

    void addPillar(Pillar pillar);

    void setTeam(Team team);

    bool isAlive() const;

    void setAlive(bool alive);

    void setPosition(const sf::Vector2f& pos);

    void move(const sf::Vector2f& delta);

    void display(size_t spriteCount) const;

    bool collision(sf::Sprite const& sprite);

    void shoot(size_t nbShoot) const;

    void setSpaceBetweenColumns(float f) { spaceBetweenColumns_ = f; }
    void setDirection(Direction       direction) { direction_   = direction; }
    void setSpriteCount(size_t        count) { spriteCount_     = count; }

    std::vector<Pillar> getGrid() const { return grid_; }
    Direction           getDirection() const { return direction_; }
    float               getWidth() const { return width_; }
    float               getHeight() const;
    sf::Vector2f        getPosition() const { return grid_.front().getPosition(); }
    size_t              getSpriteCount() const { return spriteCount_; }

    //HACK
    void setWidth(float f) { width_ = f; }

private:
    std::vector<Pillar> grid_;
    float               spaceBetweenColumns_ = 0; //espace entre les colonnes

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
    explicit SpaceInvaders(sf::RenderTarget& window, DJ&       dj);
    bool     computeFrame(const sf::Time&    elapsedTime, int& score) override;
    void     drawState(sf::Sprite&           countdown) const override;

    void manageShip(const sf::Time&  elapsedTime);
    void manageGrids(const sf::Time& elapsedTime);
    void manageShoot(const sf::Time& elapsedTime);

private:
    //constantes 
    const float ECRAN_X      = 800;
    const float ECRAN_Y      = 600;
    float       borderSpace_ = 0; //espace sur le côté

    //variables de jeu
    sf::Time age_;
    int      SI_score_ = 0;

    //textures 
    std::vector<sf::Texture> textures_;

    //Grid d'ennemi
    std::vector<Grid> grids_;
    float             gridSpeed_ = 100;
    //détermine le temps de clignotement
    const sf::Time gridBlinkTime_ = sf::milliseconds(750);
    sf::Time       gridBlinkAge_;
    //détermine la limite de déplacement vers le bas
    float downLimit_ = 0;
    //détermine le sens de dernière translation (true : droite)
    bool lastTranslation_ = true;

    //vaisseau
    Entity ship_;
    float  shipSpeed_ = 800;


    //détermine le temps de clignotement
    const sf::Time shootBlinkTime_ = sf::milliseconds(100);
    sf::Time       shootBlinkAge_;
    //conteneur de tirs
    std::vector<Entity> shoots_;
    //cooldown ennemi
    float    cooldown_ = 1000;
    sf::Time ennemyShotAge_;

    //tirs
    Entity shoot_;
    bool   playerShootDoesExist_ = false;
};


#endif // !SPACE_INVADER_HPP
