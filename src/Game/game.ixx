module;

#include <SDL2/SDL.h>
#include <SDL_video.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <filesystem>
#include <imgui.h>

export module game;
export import logger;
import ecs;

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

private:

  float _millisecondsPreviousFrame = 0.0f;
  float _deltaTime = 0.0f;

  SDL_Window* _window;
  SDL_Renderer* _renderer;
  bool isRunning;
  std::filesystem::path _assetsPath = "../assets";

  int windowWidth;
  int windowHeight;
  const int _fps = 60;
  const int _millisecondsPerFrame = 1000 / _fps;
  const bool _uncapFramerate = true;
};

Game::Game() {}

void Game::initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    Logger::err("Error initializing SDL");
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
    Logger::err("Error creating SDL window");
    return;
  }

  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!_renderer) {
    Logger::err("Error creating SDL renderer");
    return;
  }

  // SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);
  isRunning = true;
}

void Game::setup() {
}

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

void Game::update() {
  if (!_uncapFramerate && (_millisecondsPreviousFrame + _millisecondsPerFrame > SDL_GetTicks())) {
    SDL_Delay(_millisecondsPreviousFrame + _millisecondsPerFrame - SDL_GetTicks());
  }
  _deltaTime = (SDL_GetTicks() - _millisecondsPreviousFrame) / 1000.0f;

  _millisecondsPreviousFrame = SDL_GetTicks();

}

void Game::render() {
  SDL_SetRenderDrawColor(_renderer, 21, 21, 21, 255);
  SDL_RenderClear(_renderer);

  SDL_RenderPresent(_renderer);
}

void Game::destroy() {
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}
