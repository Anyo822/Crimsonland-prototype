#include "game.hpp"



Game::Game()
{
	gameWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(RES_X, RES_Y), "Crimsonland", sf::Style::Close);
	gameWindow->setMouseCursorVisible(false);
	gameWindow->setFramerateLimit(MAX_FPS);

	gameView = std::make_unique<sf::View>(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(RES_X, RES_Y));
	Map = std::make_unique<sf::RectangleShape>(sf::Vector2f(MAP_RES_X, MAP_RES_Y));
	Map->setFillColor(sf::Color(169, 169, 169, 255));
	Map->setOrigin(sf::Vector2f(MAP_RES_X / 2, MAP_RES_Y / 2));

	Hero = std::make_unique<Player>(0, 0, 64, 64, "data/avatar.jpg");
	Mouse = std::make_unique<Reticle>(0, 0, 34, 34, "data/reticle.png");
	Adversary = std::make_unique<Enemy>(0, 0, 34, 34, "data/enemy.png");
	Ammo = std::make_unique<Bullet>(0, 0, 12, 12, "data/bullet.png");
}

Game::~Game()
{
    sf::sleep(sf::milliseconds(1));
	
    if(gameWindow != nullptr)
    {
        if(gameWindow->isOpen())
            gameWindow->close();
    }
}

void Game::Run()
{
	Adversary->Spawn(ENEMIES_SPAWNED);

	while(gameWindow->isOpen())
	{
		sf::Event event;
		while(gameWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				gameWindow->close();
			if(event.type == sf::Event::KeyPressed)
                if(event.key.code == sf::Keyboard::Escape)
                    gameWindow->close();
		}

		if (!Hero->alive)
			gameWindow->close();

		HandleInput();
		Render();
		sf::sleep(sf::milliseconds(1));
	}
}

void Game::HandleInput()
{
	Hero->Move();
	Mouse->Move(*gameWindow, *gameView);
	Adversary->Move(*Hero);
	Adversary->CheckCollision();
	Ammo->Move(*Hero, *Mouse, *Adversary, *gameWindow);
}

void Game::Render()
{
	gameWindow->clear(sf::Color::Black);
	gameView->setCenter(Hero->Object_Sprite.getPosition());
	gameWindow->setView(*gameView);
	Map->setPosition(Hero->Object_Sprite.getPosition());
	gameWindow->draw(*Map);
	
	Hero->Draw(*gameWindow);
	Adversary->Draw(*gameWindow);
	Ammo->Draw(*gameWindow);
	Mouse->Draw(*gameWindow);

	gameWindow->display();
}