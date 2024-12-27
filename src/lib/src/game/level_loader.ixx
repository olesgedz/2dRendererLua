module;

#include <SDL2/SDL.h>
#include <filesystem>

export module level_loader;

import ecs;
import asset_storage;

export struct Resources {
  std::shared_ptr<Registry> registry;
  std::shared_ptr<AssetStorage> assetStorage;
  std::filesystem::path assetsPath;
  SDL_Renderer* renderer;
};

export class LevelLoader {
public:
  LevelLoader();



  void loadLevel(const Resources& resources, int levelNumber);

  ~LevelLoader();
};

//_registry, _assetStorage, _assetsPath, _renderer