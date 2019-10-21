#include "objects.hpp"
#include "Settings.hpp"

Game_Object::Game_Object(int x, int y,int width, int height, std::string Image_Path)
{
    Object_Image.loadFromFile(Image_Path);
    Object_Texture.loadFromImage(Object_Image);
    Object_Sprite.setTexture(Object_Texture);
    Object_Sprite.setTextureRect(sf::IntRect(x,y,width,height));

    randomizer.Seed((unsigned long long)time(nullptr));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Player::Player(int x, int y,int width, int height, std::string Image_Path) : Game_Object(x, y, width, height, Image_Path)
{
    Object_Sprite.setOrigin(sf::Vector2f(33, 32));
    Object_Sprite.setPosition(RES_X / 2, RES_Y / 2);
}

void Player::Move()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        Object_Sprite.move(-0.1 * speed, 0 * speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
       Object_Sprite.move(0.1 * speed, 0 * speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        Object_Sprite.move(0 * speed, -0.1 * speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        Object_Sprite.move(0 * speed, 0.1 * speed);
    }
}

void Player::Draw(sf::RenderWindow& gameWindow)
{
    gameWindow.draw(Object_Sprite);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Reticle::Reticle(int x, int y,int width, int height, std::string Image_Path) : Game_Object(x, y, width, height, Image_Path)
{
    Object_Sprite.setOrigin(sf::Vector2f(17, 17));
}

void Reticle::Move(sf::RenderWindow &gameWindow, sf::View& gameView)
{
    Object_Sprite.setPosition(sf::Vector2f(sf::Mouse::getPosition(gameWindow)));
    pos_view = gameView.getCenter();
    Object_Sprite.move(pos_view - sf::Vector2f(RES_X / 2, RES_Y / 2));
}

void Reticle::Draw(sf::RenderWindow& gameWindow)
{
    gameWindow.draw(Object_Sprite);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Enemy::Enemy(int x, int y,int width, int height, std::string Image_Path) : Game_Object(x, y, width, height, Image_Path)
{
    Object_Sprite.setOrigin(sf::Vector2f(17, 17));
}

void Enemy::Move(Player& Hero)
{
    for (int i = 0; i < gameEnemies.size(); i++)
    {
        sf::Vector2f EnemyPosition = gameEnemies[i].Enemy_Sprite.getPosition();
        sf::Vector2f HeroPosition = Hero.Object_Sprite.getPosition();
        sf::Vector2f moveDir = HeroPosition - EnemyPosition;
        sf::Vector2f moveDirNorm = moveDir / float(sqrt(pow(moveDir.x, 2) + pow(moveDir.y, 2)));
        gameEnemies[i].dir = moveDirNorm;
        
        gameEnemies[i].Enemy_Sprite.move(speed * gameEnemies[i].dir.x * gameEnemies[i].speed, speed * gameEnemies[i].dir.y * gameEnemies[i].speed);

        if (gameEnemies[i].Enemy_Sprite.getGlobalBounds().intersects(Hero.Object_Sprite.getGlobalBounds()))
            Hero.alive = false;
    }
}

void Enemy::CheckCollision()
{
    for (int i = 0; i < gameEnemies.size(); i++)
    {
        Collider first(gameEnemies[i].Enemy_Sprite);

        for (int j = 0; j < gameEnemies.size(); j++)
        {
            Collider second(gameEnemies[j].Enemy_Sprite);
            first.CheckCollision(second);
        }
    }
}

void Enemy::Spawn(int enemy_quantity)
{
    
    for (int i = 0; i < enemy_quantity; i++)
    {
        gameEnemies.push_back({Object_Sprite, sf::Vector2f(0, 0), float(randomizer.Random(100, 250)) / 100});
        switch (randomizer.Random(0, 4))
        {
            case 1:
                gameEnemies.back().Enemy_Sprite.setPosition(sf::Vector2f(randomizer.Random(17, int(RES_X)), randomizer.Random(17, int(RES_Y / 6))));
                break;
            case 2:
                gameEnemies.back().Enemy_Sprite.setPosition(sf::Vector2f(randomizer.Random(17, int(RES_X)), randomizer.Random(int(RES_Y * 5 / 6), int(RES_Y))));
                break;
            case 3:
                gameEnemies.back().Enemy_Sprite.setPosition(sf::Vector2f(randomizer.Random(17, int(RES_X / 6)), randomizer.Random(17, int(RES_Y))));
                break;
            case 4:
                gameEnemies.back().Enemy_Sprite.setPosition(sf::Vector2f(randomizer.Random(int(RES_X * 5 / 6), int(RES_X)), randomizer.Random(17, int(RES_Y))));
                break;
        }
    }
}

void Enemy::Draw(sf::RenderWindow& gameWindow)
{
    for (int i = 0; i < gameEnemies.size(); i++)
    {
        gameWindow.draw(gameEnemies[i].Enemy_Sprite);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Collider::Collider(sf::Sprite& body) : body(body)
{

}

void Collider::Move(float dx, float dy)
{
    body.move(dx, dy);
}

bool Collider::CheckCollision(Collider& other)
{
    sf::Vector2f otherPosition = other.GetPosition();
    sf::Vector2f otherHalfSize = other.GetHalfSize();
    sf::Vector2f thisPosition = GetPosition();
    sf::Vector2f thisHalfSize = GetHalfSize();

    float deltaX = otherPosition.x - thisPosition.x;
    float deltaY = otherPosition.y - thisPosition.y;
    float intersectX = abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
    float intersectY = abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

    if (intersectX < 0.0f && intersectY < 0.0f)
    {
        if (intersectX > intersectY)
        {
            if (deltaX > 0.0f)
            {
                Move(intersectX, 0.0f);
                other.Move(-intersectX, 0.0f);
            }
            else
            {
                Move(-intersectX, 0.0f);
                other.Move(intersectX, 0.0f);
            }
        }
        else
        {
            if (deltaY > 0.0f)
            {
                Move(0.0f, intersectY);
                other.Move(0.0f, -intersectY);
            }
            else
            {
                Move(0.0f, -intersectY);
                other.Move(0.0f, intersectY);
            }
        }

        return true;
    }

    return true;
}

sf::Vector2f Collider::GetPosition()
{
    return body.getPosition(); 
}

sf::Vector2f Collider::GetHalfSize()
{
    return sf::Vector2f(17, 17);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Bullet::Bullet(int x, int y,int width, int height, std::string Image_Path) : Game_Object(x, y, width, height, Image_Path)
{
    Object_Sprite.setOrigin(sf::Vector2f(6, 6));
}

void Bullet::Move(Player& Hero, Reticle& Mouse, Enemy& Adversary, sf::RenderWindow& gameWindow)
{
    if (timeElapsed < 10)
        timeElapsed++;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && timeElapsed >= 10)
    {
        Object_Sprite.setPosition(Hero.Object_Sprite.getPosition());
        projectiles.push_back({sf::Sprite(Object_Sprite), sf::Vector2f(1, 1)});

        sf::Vector2f bulletPosition = projectiles.back().Bullet_Sprite.getPosition();
        sf::Vector2f CursorPosition = Mouse.Object_Sprite.getPosition();
        sf::Vector2f aimDir = CursorPosition - bulletPosition;
        sf::Vector2f aimDirNorm = aimDir / float(sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)));
        projectiles.back().dir = aimDirNorm;

        timeElapsed = 0;

        if (projectiles.size() > MAX_AMMO)
        {
            projectiles.erase(projectiles.begin());
        }
    }

    for (int i = 0; i < projectiles.size(); i++)
    {
        projectiles[i].Bullet_Sprite.move(projectiles[i].dir.x * speed, projectiles[i].dir.y * speed);

        for (int j = 0; j < Adversary.gameEnemies.size(); j++)
        {
            if (projectiles[i].Bullet_Sprite.getGlobalBounds().intersects(Adversary.gameEnemies[j].Enemy_Sprite.getGlobalBounds()))
            {
                projectiles.erase(projectiles.begin() + i);
                Adversary.gameEnemies.erase(Adversary.gameEnemies.begin() + j);
            }
            
        }
    }
}

void Bullet::Draw(sf::RenderWindow& gameWindow)
{
    for (int i = 0; i < projectiles.size(); i++)
    {
        gameWindow.draw(projectiles[i].Bullet_Sprite);
    }
}
