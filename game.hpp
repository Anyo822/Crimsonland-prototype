#pragma once

#include <SFML\Graphics.hpp>
#include "objects.hpp"

class Game
{
public:
   Game();
   ~Game();

   void Run();

protected:
   void HandleInput();
   void Render();

private:
   std::unique_ptr <sf::RenderWindow> gameWindow;
   std::unique_ptr <sf::View> gameView;
   std::unique_ptr <Player> Hero;
   std::unique_ptr <Reticle> Mouse;
   std::unique_ptr <Enemy> Adversary;
   std::unique_ptr <Bullet> Ammo;
   std::unique_ptr <sf::RectangleShape> Map;
};
