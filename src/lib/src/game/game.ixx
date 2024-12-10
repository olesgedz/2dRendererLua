module;
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_video.h>


#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>

export module game;

export import logger;
import ecs;
import asset_storage;
import event_bus;
import components;
// import systems;
import events;
import settings;

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

  static int mapWidth;
  static int mapHeight;

private:
  std::unique_ptr<Registry> _registry;
  std::unique_ptr<AssetStorage> _assetStorage;
  std::unique_ptr<EventBus> _eventBus;

  float _millisecondsPreviousFrame = 0.0f;
  float _deltaTime = 0.0f;

  SDL_Window* _window;
  SDL_Renderer* _renderer;
  SDL_Rect _camera;

  bool _isRunning;
  bool _isDebug;

  std::filesystem::path _assetsPath = "../assets";

  const int _fps = 60;
  const int _millisecondsPerFrame = 1000 / _fps;
  const bool _uncapFramerate = true;

  ImGuiIO _io;
};