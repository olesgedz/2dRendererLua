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
import ecs;
import assetStorage;

import transformComponent;
import rigidBodyComponent;
import animationComponent;
import boxColliderComponent;

import renderSystem;
import movementSystem;
import animationSystem;
import collisionSystem;
import debugColliderSystem;

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
  std::unique_ptr<EventBus> _eventBus;

  float _millisecondsPreviousFrame = 0.0f;
  float _deltaTime = 0.0f;

  SDL_Window* _window;
  SDL_Renderer* _renderer;

  bool _isRunning;
  bool _isDebug;

  std::filesystem::path _assetsPath = "../assets";

  int _windowWidth;
  int _windowHeight;
  const int _fps = 60;
  const int _millisecondsPerFrame = 1000 / _fps;
  const bool _uncapFramerate = true;
};

Game::Game() {
  _isRunning = false;
  _isDebug = false;

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

  _windowWidth = 800;
  _windowHeight = 600;
  // Create a window
  _window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowWidth, _windowHeight,
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
  _isRunning = true;
}

void Game::setup() {
}

void Game::loadLevel(int level) {
  _registry->addSystem<MovementSystem>();
  _registry->addSystem<RenderSystem>();
  _registry->addSystem<AnimationSystem>();
  _registry->addSystem<CollisionSystem>();
  _registry->addSystem<DebugColliderSystem>();

  _assetStorage->addTexture("tank-image", _assetsPath / "images/tank-panther-right.png", _renderer);
  _assetStorage->addTexture("truck-image", _assetsPath / "images/truck-ford-right.png", _renderer);

  // Load TileMap
  _assetStorage->addTexture("jungle-tilemap", _assetsPath / "tilemaps/jungle.png", _renderer);
  _assetStorage->addTexture("chopper-spritesheet", _assetsPath / "images/chopper-spritesheet.png", _renderer);
  _assetStorage->addTexture("radar-spritesheet", _assetsPath / "images/radar.png", _renderer);

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
      tile.addComponent<SpriteComponent>("jungle-tilemap", 0, glm::vec2(tileSize, tileSize), glm::vec4(0),
                                         glm::vec2(srcRectX, srcRectY));
    }
  }

  mapFile.close();

  // UI
  Entity radar = _registry->createEntity();
  radar.addComponent<TransformComponent>(glm::vec2(_windowWidth - 74.f, 10.f), glm::vec2(1.0f, 1.0f), 0.f);
  radar.addComponent<SpriteComponent>("radar-spritesheet", 5, glm::vec2(64.f, 64.f));
  radar.addComponent<AnimationComponent>(8, 5, true);

  // Entities
  Entity chopper = _registry->createEntity();

  chopper.addComponent<TransformComponent>(glm::vec2(100.0f, 100.f), glm::vec2(1.0f, 1.0f), 0.f);
  chopper.addComponent<RigidBodyComponent>(glm::vec2(0.f, 0.f));
  chopper.addComponent<SpriteComponent>("chopper-spritesheet", 1, glm::vec2(32.f, 32.f));
  chopper.addComponent<AnimationComponent>(2, 15, true);

  Entity tank = _registry->createEntity();

  tank.addComponent<TransformComponent>(glm::vec2(300.0f, 10.f), glm::vec2(1.0f, 1.0f), 0.f);
  tank.addComponent<RigidBodyComponent>(glm::vec2(-30.0f, 0.f));
  tank.addComponent<SpriteComponent>("tank-image", 1, glm::vec2(32.f, 32.f));
  tank.addComponent<BoxColliderComponent>(glm::vec2(32.f, 32.f));

  Entity truck = _registry->createEntity();

  truck.addComponent<TransformComponent>(glm::vec2(10.0f, 10.f), glm::vec2(1.0f, 1.0f), 0.f);
  truck.addComponent<RigidBodyComponent>(glm::vec2(20.0f, 0.f));
  truck.addComponent<SpriteComponent>("truck-image", 1, glm::vec2(32.f, 32.f));
  truck.addComponent<BoxColliderComponent>(glm::vec2(32.f, 32.f));
}

void Game::run() {
  setup();
  while (_isRunning) {
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
        _isRunning = false;
        break;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) _isRunning = false;
        if (event.key.keysym.sym == SDLK_p) {
          _isDebug = !_isDebug;
          Logger::log("Debug colliders render enabled");
        }
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
  _registry->getSystem<CollisionSystem>().update();

  //Update the registry to process the entities to be added or killed
  _registry->update();
}

void Game::render() {
  SDL_SetRenderDrawColor(_renderer, 21, 21, 21, 255);
  SDL_RenderClear(_renderer);

  _registry->getSystem<AnimationSystem>().update(_deltaTime);
  _registry->getSystem<RenderSystem>().update(_renderer, _assetStorage);

  if (_isDebug) {
    _registry->getSystem<DebugColliderSystem>().update(_renderer);
  }

  SDL_RenderPresent(_renderer);
}

void Game::destroy() {
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}
