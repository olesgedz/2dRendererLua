module;
#include <SDL_ttf.h>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

export module systems:health_bar_render_system;

import components;
import logger;
import ecs;
import asset_storage;

export class HealthBarRenderSystem : public System {
public:
  HealthBarRenderSystem();
  void update(SDL_Renderer* renderer, const std::shared_ptr<AssetStorage> assetStorage, const SDL_Rect& camera) const;
};

HealthBarRenderSystem::HealthBarRenderSystem() {
  requireComponent<TransformComponent>();
  requireComponent<SpriteComponent>();
  requireComponent<HealthComponent>();
}

void HealthBarRenderSystem::update(SDL_Renderer* renderer,
                                   const std::shared_ptr<AssetStorage> assetStorage,
                                   const SDL_Rect& camera) const {
  for (auto& entity : getSystemEntities()) {
    const auto transform = entity.getComponent<TransformComponent>();
    const auto sprite = entity.getComponent<SpriteComponent>();
    const auto health = entity.getComponent<HealthComponent>();

    SDL_Color healthColor = {255, 255, 255};

    if (health.healthPercentage >= 0 && health.healthPercentage <= 40) {
      healthColor = {255, 0, 0};
    } else if (health.healthPercentage >= 40 && health.healthPercentage <= 80) {
      healthColor = {255, 255, 0};
    } else {
      healthColor = {0, 255, 0};
    }

    // Position the health bar above the entity
    int healthBarWidth = 15;
    int healthBarHeight = 3;
    double healthBarPosX = (transform.position.x + (sprite.size.x * transform.scale.x)) - camera.x;
    double healthBarPosY = (transform.position.y) - camera.y;

    SDL_Rect healthBarRect = {
        static_cast<int>(healthBarPosX),
        static_cast<int>(healthBarPosY),
        static_cast<int>(healthBarWidth * (health.healthPercentage / 100.0)),
        static_cast<int>(healthBarHeight)
    };
    SDL_SetRenderDrawColor(renderer, healthColor.r, healthColor.g, healthColor.b, 255);
    SDL_RenderFillRect(renderer, &healthBarRect);
    std::string healthText = std::to_string(health.healthPercentage);
    SDL_Surface* surface = TTF_RenderText_Blended(
        assetStorage->getFont("pico8-font-5"),
        healthText.c_str(),
        healthColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    int labelWith = 0;
    int labelHeight = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &labelWith, &labelHeight);
    SDL_Rect healthBarTextRect = {
        static_cast<int>(healthBarPosX),
        static_cast<int>(healthBarPosY) + 5,
        labelWith,
        labelHeight
    };

    SDL_RenderCopy(renderer, texture, nullptr, &healthBarTextRect);
    SDL_DestroyTexture(texture);
  }
}