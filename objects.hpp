#pragma once

#include <vector>
#include <SFML\Graphics.hpp>
#include "Randomizer.hpp"
#include "Settings.hpp"

class Game_Object
{
public:
   Game_Object(int x, int y,int width, int height, std::string Image_Path);

public:
   sf::Sprite Object_Sprite;
   sf::Image Object_Image;
   sf::Texture Object_Texture;
   Randomizer randomizer;
   float timeElapsed;
};

class Player : public Game_Object
{
public:
   Player(int x, int y,int width, int height, std::string Image_Path);

   void Move();
   void Draw(sf::RenderWindow& gameWindow);

public: 
   bool alive = true;

private:
   static constexpr float speed = 10.0f;
};

class Reticle : public Game_Object
{
public:
   Reticle(int x, int y,int width, int height, std::string Image_Path);
    
   void Move(sf::RenderWindow& gameWindow, sf::View& gameView);
   void Draw(sf::RenderWindow& gameWindow);

private:
   sf::Vector2f pos_view;
};

struct single_enemy
{
   sf::Sprite Enemy_Sprite;
   sf::Vector2f dir;
   float speed;
};

class Enemy : public Game_Object
{
public:
   Enemy(int x, int y,int width, int height, std::string Image_Path);

   void Move(Player& Hero);
   void CheckCollision();
   void Spawn(int enemy_quantity);
   void Draw(sf::RenderWindow& gameWindow);

public:
   std::vector <single_enemy> gameEnemies;

protected:
   static constexpr float speed = 0.5f;
};

class Collider
{
public:
   Collider(sf::Sprite& body);

   void Move(float dx, float dy);
   bool CheckCollision(Collider& other);
   sf::Vector2f GetPosition();
   sf::Vector2f GetHalfSize();

public:
   sf::Sprite& body;
};

struct single_bullet
{
   sf::Sprite Bullet_Sprite;
   sf::Vector2f dir;
};

class Bullet : public Game_Object
{
public:
   Bullet(int x, int y,int width, int height, std::string Image_Path);

   void Move(Player& Hero, Reticle& Mouse, Enemy& Adversary, sf::RenderWindow& gameWindow);
   void Draw(sf::RenderWindow& gameWindow);

public:
   std::vector <single_bullet> projectiles;

protected:
   static constexpr float speed = 1.5f;
};