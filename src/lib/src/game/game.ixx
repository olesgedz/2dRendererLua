module;
#include <imgui.h>
#include <SDL2/SDL.h>
#include <SDL_video.h>
#include <filesystem>
#include <sol/sol.hpp>

export module game;

export import logger;
import ecs;
import asset_storage;
import event_bus;
import components;
import events;
import settings;

export class Game {
public:
  Game();
  ~Game();

  void initialize();
  void run();
  void processInput();
  void setup();
  void update();
  void render();
  void destroy();

  static int mapWidth;
  static int mapHeight;

private:
  std::shared_ptr<Registry> _registry;
  std::shared_ptr<AssetStorage> _assetStorage;
  std::shared_ptr<EventBus> _eventBus;

  float _millisecondsPreviousFrame = 0.0f;
  float _deltaTime = 0.0f;

  SDL_Window* _window;
  SDL_Renderer* _renderer;
  SDL_Rect _camera;

  std::shared_ptr<sol::state> _lua;

  bool _isRunning;
  bool _isDebug;

  std::filesystem::path _assetsPath = "../assets";

  const int _fps = 60;
  const int _millisecondsPerFrame = 1000 / _fps;
  const bool _uncapFramerate = true;

  ImGuiIO _io;
};