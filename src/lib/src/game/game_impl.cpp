module;
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_video.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>

module game;

import logger;
import ecs;
import asset_storage;
import event_bus;
import components;
import systems;
import events;
import settings;
import level_loader;

int Game::mapWidth = 0;
int Game::mapHeight = 0;

Game::Game() {
  _isRunning = false;
  _isDebug = false;

  _registry = std::make_shared<Registry>();
  _assetStorage = std::make_shared<AssetStorage>();
  _eventBus = std::make_shared<EventBus>();

  Logger::log("Game constructor called!");
}

Game::~Game() { Logger::log("Game destructor called!"); }

void Game::initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::string error = SDL_GetError();
    throw std::runtime_error("Error initializing SDL error: " + error);
  }

  if (TTF_Init() != 0) {
    std::string error = TTF_GetError();
    throw std::runtime_error("Error initializing TTF error: " + error);
  }

  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);

  Settings::windowWidth = 800;
  Settings::windowHeight = 600;

  // Create a window
  _window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Settings::windowWidth,
                             Settings::windowHeight, SDL_WINDOW_RESIZABLE);

  if (!_window) {
    throw std::runtime_error("Error creating SDL window");
  }

  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!_renderer) {
    throw std::runtime_error("Error creating SDL renderer");
  }
  IMGUI_CHECKVERSION();
  // init ImGui
  ImGui::CreateContext();
  _io = ImGui::GetIO();
  (void)_io;
  _io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  _io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForSDLRenderer(_window, _renderer);
  ImGui_ImplSDLRenderer2_Init(_renderer);

  _camera.x = 0;
  _camera.y = 0;
  _camera.w = Settings::windowWidth;
  _camera.h = Settings::windowHeight;

  // SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);
  _isRunning = true;
}

void Game::setup() {
  _registry->addSystem<MovementSystem>();
  _registry->addSystem<RenderSystem>();
  _registry->addSystem<AnimationSystem>();
  _registry->addSystem<CollisionSystem>();
  _registry->addSystem<DebugColliderSystem>();
  _registry->addSystem<DamageSystem>();
  _registry->addSystem<KeyboardControlSystem>();
  _registry->addSystem<CameraMovementSystem>();
  _registry->addSystem<ProjectileEmitSystem>();
  _registry->addSystem<ProjectileLifecycleSystem>();
  _registry->addSystem<RenderTextSystem>();
  _registry->addSystem<HealthBarRenderSystem>();
  _registry->addSystem<RenderGUISystem>();

  LevelLoader loader;
  Resources resources;
  resources.assetStorage = _assetStorage;
  resources.registry = _registry;
  resources.renderer = _renderer;
  resources.assetsPath = _assetsPath;
  loader.loadLevel(resources, 1);
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
    ImGui_ImplSDL2_ProcessEvent(&event);

    int mouseX, mouseY;
    const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

    switch (event.type) {
      case SDL_QUIT:
        _isRunning = false;
        break;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) _isRunning = false;
        if (event.key.keysym.sym == SDLK_p) {
          _isDebug = !_isDebug;
          if (_isDebug) {
            Logger::log("Debug colliders render enabled");
          } else {
            Logger::log("Debug colliders render disabled");
          }
        }
        _eventBus->emitEvent<KeyPressEvent>(event.key.keysym.sym);
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
  _registry->getSystem<KeyboardControlSystem>().update();

  // Reset eventbus
  _eventBus->reset();

  // Subscriptions
  _registry->getSystem<DamageSystem>().subscribeToEvents(_eventBus);
  _registry->getSystem<MovementSystem>().subscribeToEvents(_eventBus);
  _registry->getSystem<KeyboardControlSystem>().subscribeToEvents(_eventBus);
  _registry->getSystem<ProjectileEmitSystem>().subscribeToEvents(_eventBus);

  // Update the registry to process the entities to be added or killed
  _registry->update();

  // Systems updates
  _registry->getSystem<MovementSystem>().update(_deltaTime);
  _registry->getSystem<AnimationSystem>().update(_deltaTime);
  _registry->getSystem<CollisionSystem>().update(_eventBus);
  _registry->getSystem<DamageSystem>().update();
  _registry->getSystem<CameraMovementSystem>().update(_camera);
  _registry->getSystem<ProjectileEmitSystem>().update(_registry);
  _registry->getSystem<ProjectileLifecycleSystem>().update();
}

void Game::render() {
  SDL_SetRenderDrawColor(_renderer, 21, 21, 21, 255);
  SDL_RenderClear(_renderer);

  _registry->getSystem<RenderSystem>().update(_renderer, _assetStorage, _camera);
  _registry->getSystem<RenderTextSystem>().update(_renderer, _assetStorage, _camera);
  _registry->getSystem<HealthBarRenderSystem>().update(_renderer, _assetStorage, _camera);

  if (_isDebug) {
    _registry->getSystem<DebugColliderSystem>().update(_renderer, _camera);
    _registry->getSystem<RenderGUISystem>().update(_renderer, _io, _registry);
  }
  SDL_RenderPresent(_renderer);
}

void Game::destroy() {
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  TTF_Quit();
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}