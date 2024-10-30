module;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cassert>
#include <filesystem>
#include <string>
#include <unordered_map>

module asset_storage;

AssetStorage::~AssetStorage() {
  clearAssets();
  Logger::log("AssetStorage destructor called!");
}

void AssetStorage::clearAssets() {
  for (auto& texture : _textures) {
    SDL_DestroyTexture(texture.second);
  }
  _textures.clear();
}

void AssetStorage::addTexture(const std::string& id, std::filesystem::path path, SDL_Renderer* renderer) {
  SDL_Surface* surface = IMG_Load(path.string().c_str());
  if (!surface) {
    throw std::runtime_error("Failed to load texture: " + path.string());
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  assert(renderer != nullptr && "Renderer is nullptr");

  if (!texture || !renderer) {
    throw std::runtime_error("Failed to create texture from surface: " + path.string());
  }

  SDL_FreeSurface(surface);
  _textures.emplace(id, texture);

  Logger::log("New texture added to assetStorage with id of: " + id + "\n and path of: " + path.string());
}

SDL_Texture* AssetStorage::getTexture(const std::string& id) const {
  assert(_textures.contains(id) && "Texture not found in assetStorage");
  return _textures.at(id);
}