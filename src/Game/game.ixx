module;

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_video.h>
#include <imgui.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>

export module game;
export import logger;
import transformComponent;
import rigidBodyComponent;
import movementSystem;
import ecs;
import renderSystem;
import assetStorage;

export class Game {
 public:
  Game();
 ~Game();

  void initialize();
  void loadLevel(int level);
  void run();
  void processInput();
  void setup();
  void update();
  void render();
  void destroy();

 private:
  std::unique_ptr<Registry> _registry;
  std::unique_ptr<AssetStorage> _assetStorage;

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

Game::Game() {
        isRunning = false;
        _registry = std::make_unique<Registry>();
        _assetStorage = std::make_unique<AssetStorage>();
        Logger::log("Game constructor called!");
}

Game::~Game() {
  Logger::log("Game destructor called!");
}

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
                             SDL_WINDOW_RESIZABLE);

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

void Game::loadLevel(int level) {
  _registry->addSystem<MovementSystem>();
  _registry->addSystem<RenderSystem>();

  _assetStorage->addTexture("tank-image", _assetsPath / "images/tank-panther-right.png", _renderer);
  _assetStorage->addTexture("truck-image", _assetsPath / "images/truck-ford-right.png", _renderer);
  // Load TileMap
  _assetStorage->addTexture("jungle-tilemap", _assetsPath / "tilemaps/jungle.png", _renderer);

  // Load the tilemap
  int tileSize = 32;
  double tileScale = 1.0;
  int mapNumCols = 25;
  int mapNumRows = 20;

  std::fstream mapFile;
  mapFile.open(_assetsPath / "tilemaps/jungle.map");
  if (!mapFile) {
    Logger::err("Failed to open map file");
    return;
  }

  for (int y = 0; y < mapNumRows; y++) {
    for (int x = 0; x < mapNumCols; x++) {
      char ch;
      mapFile.get(ch);
      int srcRectY = std::atoi(&ch) * tileSize;
      mapFile.get(ch);
      int srcRectX = std::atoi(&ch) * tileSize;
      mapFile.ignore();

      Entity tile = _registry->createEntity();
      tile.addComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)),
                                            glm::vec2(tileScale, tileScale), 0.0);
      tile.addComponent<SpriteComponent>("jungle-tilemap", glm::vec2(tileSize, tileSize), glm::vec4(0),
                                         glm::vec2(srcRectX, srcRectY));
    }
  }
  mapFile.close();

  Entity tank = _registry->createEntity();

  tank.addComponent<TransformComponent>(glm::vec2(10.0f, 10.f), glm::vec2(3.0f, 3.0f), 35.f);
  tank.addComponent<RigidBodyComponent>(glm::vec2(40.0f, 0.f));
  tank.addComponent<SpriteComponent>("tank-image", glm::vec2(32.f, 32.f));

  Entity truck = _registry->createEntity();

  truck.addComponent<TransformComponent>(glm::vec2(100.0f, 40.f), glm::vec2(1.0f, 1.0f), 0.f);
  truck.addComponent<RigidBodyComponent>(glm::vec2(5.0f, 30.f));
  truck.addComponent<SpriteComponent>("truck-image", glm::vec2(32.f, 32.f));
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

  //Systems updates
  _registry->getSystem<MovementSystem>().update(_deltaTime);

  //Update the registry to process the entities to be added or killed
  _registry->update();
}

void Game::render() {
  SDL_SetRenderDrawColor(_renderer, 21, 21, 21, 255);
  SDL_RenderClear(_renderer);

  _registry->getSystem<RenderSystem>().update(_renderer, _assetStorage);

  SDL_RenderPresent(_renderer);
}

void Game::destroy() {
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}
