#pragma once
#include <SDL2/SDL.h>
#include <SDL_video.h>

class Game {
 public:
  Game();

  void initialize();
  void run();
  void processInput();
  void setup();
  void update();
  void render();
  void destroy();

  int windowWidth;
  int windowHeight;

 private:
  SDL_Window* _window;
  SDL_Renderer* _renderer;
  bool isRunning;
};
