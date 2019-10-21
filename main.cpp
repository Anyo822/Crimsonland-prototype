#include <SFML\Graphics.hpp>
#include "game.hpp"

int main (int argc, char* argv[])
{
    std::unique_ptr<Game> Crland = std::make_unique<Game>();
    Crland->Run();

    return 0;
}