#include "SpaceInvaders.hpp"
#include "../../constants.hpp"
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

Entity::Entity(sf::RenderTarget* window, const sf::Texture& texture): window_{window}
{
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprites_.push_back(sprite);
}

void Entity::addSprite(const sf::Texture& texture)
{
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprites_.push_back(sprite);
}

void Entity::centerSpriteOrigin()
{
    for(auto& sprite : sprites_)
        sprite.setOrigin(sprite.getGlobalBounds().width / 2., sprite.getGlobalBounds().height / 2.);
}

void Entity::rotate(float angle)
{
    for(auto& sprite : sprites_)
        sprite.rotate(angle);
}

void Entity::display() const
{
    if(alive_)
        window_->draw(sprites_.front());
}

void Entity::display(size_t spriteCount) const
{
    if(alive_)
        window_->draw(sprites_[spriteCount]);
}

void Entity::move(const sf::Vector2f& delta)
{
    for(auto& sprite : sprites_)
        sprite.move(delta.x, delta.y);
}

void Entity::setPosition(const sf::Vector2f& pos)
{
    for(auto& sprite : sprites_)
        sprite.move(pos - sprite.getPosition());
}

bool Entity::collision(sf::Sprite const& sprite)
{
    if(alive_ && sprite.getGlobalBounds().intersects(sprites_.front().getGlobalBounds()))
    {
        alive_ = false;
        return true;
    }
    return false;
}

bool Entity::collision(Entity const& entity) { return collision(entity.getSprites().front()); }

void Pillar::setTeam(Team team)
{
    for(auto& invader : pillar_)
        invader.setTeam(team);
}

bool Pillar::isAlive() const
{
    for(const auto& e : pillar_)
        if(e.isAlive())
            return true;
    return false;
}

void Pillar::setAlive(bool alive)
{
    for(auto& invader : pillar_)
        invader.setAlive(alive);
}

void Pillar::addInvader(const Entity& invader)
{
    //largeur
    if(invader.getGlobalBounds().width > width_)
        width_ = invader.getGlobalBounds().width;
    //hauteur
    height_ += invader.getGlobalBounds().height;
    if(!pillar_.empty())
        height_ += spaceBetweenLines_;

    pillar_.push_back(invader);
    setPosition({0, 0});
}

void Pillar::setPosition(const sf::Vector2f& pos)
{
    float     y = pos.y;
    float     x = pos.x;
    for(auto& invader : pillar_)
    {
        invader.setPosition({x, y});
        y += spaceBetweenLines_ + invader.getGlobalBounds().height;
    }
}

void Pillar::move(const sf::Vector2f& delta)
{
    for(auto& invader : pillar_)
        invader.move(delta);
}

void Pillar::display(size_t spriteCount) const
{
    for(auto& invader : pillar_)
        invader.display(spriteCount);
}

bool Pillar::collision(sf::Sprite const& sprite)
{
    for(auto& invader : pillar_)
    {
        if(invader.collision(sprite))
            return true;
    }
    return false;
}

float Pillar::getHeight() const
{
    // float height= 0;
    // for(const auto& invader : pillar_)
    //     if(invader.isAlive()) height += invader.getGlobalBounds().height + spaceBetweenLines_;
    return height_;
}

sf::Vector2f Pillar::getPillarBottomPos() const
{
    sf::Vector2f    pos = {0, 0};
    for(const auto& invader : pillar_)
    {
        if(invader.isAlive() && invader.getPosition().y > pos.y)
        {
            pos.y = invader.getPosition().y + invader.getGlobalBounds().height;
            pos.x = invader.getPosition().x + invader.getGlobalBounds().width / 2.f;
        }
    }

    return pos;
}

void Grid::addPillar(Pillar pillar)
{
    grid_.push_back(pillar);
    setPosition({0, 0});

    //hauteur
    if(pillar.getHeight() > height_)
        height_ = pillar.getHeight();
    //largeur
    width_ += spaceBetweenColumns_;
}

void Grid::setTeam(Team team)
{
    for(auto& pillar : grid_)
        pillar.setTeam(team);
}

bool Grid::isAlive() const
{
    for(const auto& pillar : grid_)
        if(pillar.isAlive())
            return true;
    return false;
}

void Grid::setAlive(bool alive)
{
    for(auto& pillar : grid_)
        pillar.setAlive(alive);
}

void Grid::setPosition(const sf::Vector2f& pos)
{
    float     y = pos.y;
    float     x = pos.x;
    for(auto& pillar : grid_)
    {
        pillar.setPosition({x, y});
        x += pillar.getWidth() + spaceBetweenColumns_;
    }
}

void Grid::move(const sf::Vector2f& delta)
{
    for(auto& pillar : grid_)
        pillar.move(delta);
}

void Grid::display(size_t spriteCount) const
{
    for(auto& pillar : grid_)
        pillar.display(spriteCount);
}

bool Grid::collision(sf::Sprite const& sprite)
{
    for(auto& pillar : grid_)
    {
        if(pillar.collision(sprite))
            return true;
    }
    return false;
}

void Grid::shoot(size_t nbShoot) const
{
    if(nbShoot > grid_.size())
        throw std::runtime_error("Out of grid");
}

float Grid::getHeight() const
{
    //return std::max_element(grid_.begin(), grid_.end(), [](Pillar const& a, Pillar const& b){ return a.getHeight() < b.getHeight(); })->getHeight();
    return height_;
} 

SpaceInvaders::SpaceInvaders(sf::RenderTarget& window, DJ& dj) : Game{window, dj}
{
    textures_.resize(18);

    //vaisseau
    textures_[0].loadFromFile(location "img/SpaceInvaders/ship.png");
    ship_ = Entity(&window, textures_[0]);
    ship_.move({ECRAN_X / 2, ECRAN_Y - ship_.getGlobalBounds().height}); //initial pos
    ship_.setTeam(SI_FRIEND);
    borderSpace_ = std::ceil(ship_.getGlobalBounds().width / 2.f);

    //TEXTURES des ennemis
    //invader haut
    textures_[1].loadFromFile(location "img/SpaceInvaders/invaderUpA.png");
    textures_[2].loadFromFile(location "img/SpaceInvaders/invaderUpB.png");
    //invader mileu
    textures_[3].loadFromFile(location "img/SpaceInvaders/invaderMiddleA.png");
    textures_[4].loadFromFile(location "img/SpaceInvaders/invaderMiddleB.png");
    //invadeur bas
    textures_[5].loadFromFile(location "img/SpaceInvaders/invaderDownA.png");
    textures_[6].loadFromFile(location "img/SpaceInvaders/invaderDownB.png");
    //invader Boss
    //TODO PG à augmenter x8 depuis le dossier Original
    //textures_[7].loadFromFile(location "img/SpaceInvaders/invaderDownB.png");

    //TEXTURES des protections
    textures_[8].loadFromFile(location "img/SpaceInvaders/prot.png");
    textures_[9].loadFromFile(location "img/SpaceInvaders/prot1.png");
    textures_[10].loadFromFile(location "img/SpaceInvaders/prot2.png");
    textures_[11].loadFromFile(location "img/SpaceInvaders/prot3.png");
    //TODO PG à augmenter x8 depuis le dossier Original
    //textures_[12].loadFromFile(location "img/SpaceInvaders/protHalf.png");
    //textures_[13].loadFromFile(location "img/SpaceInvaders/protHalf1.png");
    //textures_[14].loadFromFile(location "img/SpaceInvaders/protHalf2.png");
    //textures_[15].loadFromFile(location "img/SpaceInvaders/protHalf3.png");

    //TEXTURE du tir
    textures_[16].loadFromFile(location "img/SpaceInvaders/shootA.png");

    textures_[17].loadFromFile(location "img/SpaceInvaders/bg.png");

    //Création du tir
    shoot_ = Entity(&window, textures_[16]);
    shoot_.centerSpriteOrigin();

    //Création de la Grid A
    Grid gridA;
    //pillar 1
    Pillar pillarA1;
    Entity entityA11(&window, textures_[1]);
    entityA11.addSprite(textures_[2]);
    Entity entityA12(&window, textures_[3]);
    entityA12.addSprite(textures_[4]);
    Entity entityA13(&window, textures_[3]);
    entityA13.addSprite(textures_[4]);
    Entity entityA14(&window, textures_[5]);
    entityA14.addSprite(textures_[6]);
    Entity entityA15(&window, textures_[5]);
    entityA15.addSprite(textures_[6]);
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
    ////pillar 6 
    //gridA.addPillar(pillarA1);
    ////pillar 7
    //gridA.addPillar(pillarA1);
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

    for(auto& grid : grids_)
    {
        float     gridWidth = 0;
        for(auto& pillar : grid.getGrid()) { gridWidth += pillar.getWidth(); }
        grid.setSpaceBetweenColumns((ECRAN_X - 2 * borderSpace_ - gridWidth) / grid.getGrid().size());
        grid.setWidth(ECRAN_X - 2 * borderSpace_ - (ECRAN_X - 2 * borderSpace_ - gridWidth) / grid.getGrid().size());
        grid.setPosition({0, 30});
        grid.setTeam(SI_ENNEMY);
    }
}

void SpaceInvaders::manageShip(const sf::Time& elapsedTime)
{
    float deltaX = 0;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        deltaX = -shipSpeed_ * elapsedTime.asSeconds();
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        deltaX = shipSpeed_ * elapsedTime.asSeconds();

    const float taille = ship_.getGlobalBounds().width;
    const float pos    = ship_.getPosition().x;
    if((pos + taille + deltaX) > ECRAN_X)
        deltaX = ECRAN_X - pos - taille;
    else if(pos + deltaX < 0)
        deltaX = -pos;
    ship_.move({deltaX, 0});
}

void SpaceInvaders::manageGrids(const sf::Time& elapsedTime)
{
    if(gridBlinkAge_ >= gridBlinkTime_)
    {
        //HACK nombre de sprites
        size_t nbSprite = 2;

        grids_[0].setSpriteCount(grids_[0].getSpriteCount() + 1);
        if(grids_[0].getSpriteCount() >= nbSprite)
            grids_[0].setSpriteCount(0);
        gridBlinkAge_ = sf::Time::Zero;
    }
    gridBlinkAge_ += elapsedTime;

    //màj de la vitesse de la grid
    gridSpeed_ = 300;

    //mouvement : création du delta de mouvement
    sf::Vector2f delta = {
        gridSpeed_ * elapsedTime.asSeconds() * static_cast<float>(grids_[0].getDirection() == DIR_RIGHT) - gridSpeed_ *
        elapsedTime
        .asSeconds() * static_cast<float>(grids_[0].getDirection() == DIR_LEFT),
        gridSpeed_ * elapsedTime.asSeconds() * static_cast<float>(grids_[0].getDirection() == DIR_DOWN) - gridSpeed_ *
        elapsedTime.
        asSeconds() * static_cast<float>(grids_[0].getDirection() == DIR_UP)
    };
    const sf::Vector2f taille = {grids_[0].getWidth(), grids_[0].getHeight()};
    const sf::Vector2f pos    = grids_[0].getPosition();

    if((pos.x + taille.x + delta.x) > ECRAN_X) //trop à droite
    {
        delta.x = ECRAN_X - pos.x - taille.x;
        grids_[0].setDirection(DIR_DOWN);
    }
    else if(pos.x + delta.x < 0) //trop à gauche
    {
        delta.x = -pos.x;
        grids_[0].setDirection(DIR_DOWN);
    }

    const float MGK = 60;
    //la magick constant correspond à la taille entre le bas de l'écran et le haut des abris (vaisseau + abri + espace entre les 2)
    // 8*8 + 16*8 = 192

    if(downLimit_ > (ECRAN_Y - MGK))
        downLimit_ = (ECRAN_Y - MGK);

    if((pos.y + taille.y + delta.y) >= downLimit_) //trop en bas
    {
        delta.y = downLimit_ - pos.y - taille.y - 1;
        if(lastTranslation_) //si avant on allait à droite
        {
            grids_[0].setDirection(DIR_LEFT);
            lastTranslation_ = false;
        }
        else //si avant on allait à gauche
        {
            grids_[0].setDirection(DIR_RIGHT);
            lastTranslation_ = true;
        }

        downLimit_ += SPACE_BETWEEN_LINES / 2.f;
    }
    else if(pos.y + delta.y < 0) //trop en haut
    {
        delta.y = -pos.y;
        grids_[0].setDirection(DIR_DOWN);
    }

    for(auto& grid : grids_) //màj des positions
    {
        grid.move(delta);
    }
}

void SpaceInvaders::manageShoot(const sf::Time& elapsedTime)
{
    if(shootBlinkAge_ >= shootBlinkTime_)
    {
        for(auto& shoot : shoots_)
            shoot.rotate(180);
        shootBlinkAge_ = sf::Time::Zero;
    }
    shootBlinkAge_ += elapsedTime;

    //tir ami
    if(!playerShootDoesExist_ && (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::
        isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
    {
        playerShootDoesExist_ = true;
        shoots_.push_back(shoot_);
        shoots_.back().setPosition({
            ship_.getPosition().x + ship_.getGlobalBounds().width / 2.f,
            ship_.getPosition().y
        });
        shoots_.back().setTeam(SI_FRIEND);

        Dj_.play(12, true);
    }

    //Gestion des tirs ennemis
    /*
    static std::random_device                    rd;
    static std::default_random_engine            gen(rd());
    static std::uniform_int_distribution<int>    unif_int(0, grids_[0].getGrid().size() - 1);
    static std::uniform_real_distribution<float> rand_cooldown(1000, 2000);
    static std::uniform_real_distribution<float> rand_speed(50, 200);

    if(ennemyShotAge_.asMilliseconds() > cooldown_) //actual ennemy shoot
    {
        int nbPillarShooting = unif_int(gen);
        if(nbPillarShooting == 0)
            nbPillarShooting = 1; //toujours au moins un tir ennemi par vague

        std::vector<int> shotIndex(grids_.front().getGrid().size());
        std::iota(shotIndex.begin(), shotIndex.end(), 0);
        std::shuffle(shotIndex.begin(), shotIndex.end(), gen);

        for(int i = 0 ; i < nbPillarShooting ; ++i)
        {
            shoots_.push_back(shoot_);
            shoots_.back().setPosition(grids_[0].getGrid()[shotIndex[i]].getPillarBottomPos());
            shoots_.back().setTeam(SI_ENNEMY);
            shoots_.back().setSpeed(100);
        }

        ennemyShotAge_ = sf::Time::Zero;
        cooldown_      = rand_cooldown(gen);
    }
    ennemyShotAge_ += elapsedTime;
    */

    //Gestion des tirs
    for(auto& shoot : shoots_)
    {
        if(shoot.getTeam() == SI_FRIEND)
        {
            shoot.move({0, - shoot.speed() * elapsedTime.asSeconds()});
            if(grids_.front().collision(shoot.getSprites().front())) //ennemi touché
            {
                shoot.setAlive(false);
                playerShootDoesExist_ = false;
                SI_score_ += 100;
                Dj_.getAllTrack()[5].getSound().play();
            }
            if(shoot.getPosition().y < 0)
            {
                shoot.setAlive(false);
                playerShootDoesExist_ = false;
            }
        }
        else if(shoot.getTeam() == SI_ENNEMY)
        {
            shoot.move({0, shoot.speed() * elapsedTime.asSeconds()});
            if(ship_.collision(shoot)) //vaisseau touché
            {
                shoot.setAlive(false); //ici le vaisseau allié est mort
            }
            if(shoot.getPosition().y > ECRAN_Y)
            {
                shoot.setAlive(false);
            }
            //TODO collision avec protecteur
        }
        else
            throw std::runtime_error("shoot with neutral team");
    }

    //Destruction tirs morts
    shoots_.erase(std::remove_if(shoots_.begin(), shoots_.end(), [](Entity const& shoot) { return !shoot.isAlive(); }),
                  shoots_.end());
}


bool SpaceInvaders::computeFrame(const sf::Time& elapsedTime, int& score)
{
    manageShip(elapsedTime);
    manageGrids(elapsedTime);
    manageShoot(elapsedTime);
    age_ += elapsedTime;

    score += SI_score_;
    SI_score_ = 0;

    if(!ship_.isAlive())
        return false;
    if(!grids_.front().isAlive())
    {
        score += 1000;
        return false;
    }

    return true;
}

void SpaceInvaders::drawState(sf::Sprite& countdown) const
{
    const auto         size = window_.getSize();
    sf::RectangleShape rect(window_.mapPixelToCoords({static_cast<int>(size.x), static_cast<int>(size.y)}));
    rect.setFillColor(sf::Color::Black);
    window_.draw(rect);

    sf::Sprite bg;
    bg.setTexture(textures_[17]);
    window_.draw(bg);

    window_.draw(countdown);

    //vaisseau
    ship_.display();

    //grid
    grids_[0].display(grids_[0].getSpriteCount());

    //tirs
    for(auto const& shoot : shoots_)
        shoot.display();

    //Debug
    //sf::RectangleShape line(sf::Vector2f(150, 2));
    //line.setFillColor(sf::Color::Red);
    //line.setPosition({ 0,downLimit_ });
    //window_.draw(line);
}
