module;

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <cassert>
#include <filesystem>
#include <ranges>
#include <string>

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

  for (auto& font : _fonts) {
    TTF_CloseFont(font.second);
  }
  _fonts.clear();
}

void AssetStorage::addTexture(const std::string& id, std::filesystem::path path, SDL_Renderer* renderer) {
  SDL_Surface* surface = IMG_Load(path.string().c_str());
  if (!surface) {
    Logger::err("Failed to load texture: " + path.string());
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

void AssetStorage::addFont(const std::string& assetId, std::filesystem::path path, int fontSize) {
  if (!std::filesystem::exists(path)) {
    throw std::runtime_error("Font file not found: " + path.string());
  }
  _fonts.emplace(assetId, TTF_OpenFont(path.string().c_str(), fontSize));
}

TTF_Font* AssetStorage::getFont(const std::string& assetId) {
  assert(_fonts.contains(assetId) && "Font not found in assetStorage");
  return _fonts.at(assetId);
}

std::vector<const char*> AssetStorage::getTextureIds() const {
  std::vector<const char*> textureIds;
  for (const auto& id : std::ranges::views::keys(_textures)) {
    textureIds.push_back(id.c_str());
  }
  return textureIds;
}
