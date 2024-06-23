#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <sol/sol.hpp>

#include "Game.h"

int main(int argc, char *args[]) {
    Game game;

    game.initialize();
    game.run();
    game.destroy();

    return 0;
}
