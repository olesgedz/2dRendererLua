module;
#include <SDL_ttf.h>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

export module systems:render_text_system;

import components;
import logger;
import ecs;
import asset_storage;

export class RenderTextSystem : public System {
public:
  RenderTextSystem();
  void update(SDL_Renderer* renderer, const std::unique_ptr<AssetStorage>& assetStorage, const SDL_Rect& camera) const;
};


void RenderTextSystem::update(SDL_Renderer* renderer,
                              const std::unique_ptr<AssetStorage>& assetStorage,
                              const SDL_Rect& camera) const {
  for (auto& entity : getSystemEntities()) {
    const auto& textLabel = entity.getComponent<TextLabelComponent>();
    const auto& transform = entity.getComponent<TransformComponent>();

    SDL_Surface* surface = TTF_RenderText_Blended(
        assetStorage->getFont(textLabel.assetId),
        textLabel.text.c_str(),
        textLabel.color);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    int labelWidth = 0;
    int labelHeight = 0;

    SDL_QueryTexture(texture, nullptr, nullptr, &labelWidth, &labelHeight);

    SDL_Rect dstRect = {
        static_cast<int>(textLabel.position.x - (textLabel.isFixed ? 0 : camera.x)),
        static_cast<int>(textLabel.position.y - (textLabel.isFixed ? 0 : camera.y)),
        labelWidth,
        labelHeight
    };

    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_DestroyTexture(texture);
  }
}

RenderTextSystem::RenderTextSystem() : System() {
  requireComponent<TextLabelComponent>();
}
