module;

#include <SDL2/SDL.h>
#include <filesystem>
#include <sol/sol.hpp>

export module level_loader;

import ecs;
import asset_storage;

export struct Resources {
  std::shared_ptr<Registry> registry;
  std::shared_ptr<AssetStorage> assetStorage;
  std::filesystem::path assetsPath;
  SDL_Renderer* renderer;
  std::shared_ptr<sol::state> lua;
};

export class LevelLoader {
public:
  LevelLoader();



  void loadLevel(const Resources& resources, int levelNumber);

  ~LevelLoader();
};

//_registry, _assetStorage, _assetsPath, _renderer