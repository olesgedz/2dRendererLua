#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <imgui.h>

#include <glm/glm.hpp>
#include <iostream>
#include <sol/sol.hpp>

import Game;

int main(int argc, char* args[]) {
  Game game;

  std::cout << "Initializing game..." << std::endl;
  game.initialize();
  game.run();
  game.destroy();

  return 0;
}
