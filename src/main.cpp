//#include <imgui.h>

import game;

int main(int argc, char* args[]) {
  Game game;

  Logger::log("Initializing game...");

  game.initialize();
  game.run();
  game.destroy();

  return 0;
}
