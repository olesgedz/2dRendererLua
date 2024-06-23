#include "Game.h"

#include <imgui.h>

Game::Game()
{
}

void Game::initialize()
{
}

void Game::run()
{
    while (true)
    {
        processInput();
        update();
        render();
    }
}

void Game::processInput()
{
}

void Game::update()
{
}

void Game::render()
{
}

void Game::destroy()
{
}
