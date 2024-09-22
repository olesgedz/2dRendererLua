module;

#include <SDL2/SDL.h>
#include <SDL_video.h>
#include <imgui.h>
#include <iostream>

export module Game;

export class Game {
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

Game::Game() {}

void Game::initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr << "Error initializing SDL" << std::endl;
    return;
  }
  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);

  windowWidth = 800;
  windowHeight = 600;
  // Create a window
  _window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight,
                             SDL_WINDOW_BORDERLESS);

  if (!_window) {
    std::cerr << "Error creating SDL window" << std::endl;
    return;
  }

  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!_renderer) {
    std::cerr << "Error creating SDL renderer" << std::endl;
    return;
  }

  // SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);
  isRunning = true;
}

void Game::setup() {}

void Game::run() {
  setup();
  while (isRunning) {
    processInput();
    update();
    render();
  }
}

void Game::processInput() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        isRunning = false;
      break;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
      break;
      default:
        break;
    }
  }
}

void Game::update() {}

void Game::render() {
  SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
  SDL_RenderClear(_renderer);

  SDL_RenderPresent(_renderer);
}

void Game::destroy() {
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}
