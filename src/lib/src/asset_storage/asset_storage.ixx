module;

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <filesystem>
#include <string>
#include <unordered_map>

export module asset_storage;


import logger;


export class AssetStorage {
public:
  AssetStorage() = default;
  ~AssetStorage();

  void clearAssets();

  void addTexture(const std::string& id, std::filesystem::path path, SDL_Renderer* renderer);
  [[nodiscard]] SDL_Texture* getTexture(const std::string& id) const;

  void addFont(const std::string& assetId, std::filesystem::path path, int fontSize);
  TTF_Font* getFont(const std::string& assetId);
  [[nodiscard]] std::vector<const char*> getTextureIds() const;

private:
  std::unordered_map<std::string, SDL_Texture*> _textures;
  std::unordered_map<std::string, TTF_Font*> _fonts;
};