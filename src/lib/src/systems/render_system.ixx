module;
#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

export module systems:render_system;

import components;
import logger;
import ecs;
import asset_storage;

export class RenderSystem : public System {
public:
  RenderSystem();
  void update(SDL_Renderer* renderer, const std::shared_ptr<AssetStorage>& assetStorage, const SDL_Rect& camera) const;
};

RenderSystem::RenderSystem() {
  requireComponent<TransformComponent>();
  requireComponent<SpriteComponent>();
}

void RenderSystem::update(SDL_Renderer* renderer,
                          const std::shared_ptr<AssetStorage>& assetStorage,
                          const SDL_Rect& camera) const {
  // It returns a vector of entities that have the required components, so
  // only drawable entities.
  std::vector<Entity> entities = getSystemEntities();

  // We can sort the entities on add Entity or delete
  std::ranges::sort(entities, [](Entity& a, Entity& b) {
    return a.getComponent<SpriteComponent>().zDepth < b.getComponent<SpriteComponent>().zDepth;
  });

  for (auto& entity : entities) {
    const auto& transform = entity.getComponent<TransformComponent>();
    const auto& sprite = entity.getComponent<SpriteComponent>();

    if (transform.position.x + sprite.size.x * transform.scale.x < camera.x ||
        transform.position.x > camera.x + camera.w ||
        transform.position.y + sprite.size.y * transform.scale.y < camera.y ||
        transform.position.y > camera.y + camera.h &&
        !sprite.isFixed)
      continue;
    SDL_Rect srcRect = sprite.srcRect;
    glm::vec2 shift = glm::vec2(0);

    if (!sprite.isFixed) {
      shift = glm::vec2(camera.x, camera.y);
    }

    SDL_Rect dstRect = {
        static_cast<int>(transform.position.x - shift.x),
        static_cast<int>(transform.position.y - shift.y),
        static_cast<int>(sprite.size.x * transform.scale.x),
        static_cast<int>(sprite.size.y * transform.scale.y)
    };

    SDL_RenderCopyEx(renderer,
                     assetStorage->getTexture(sprite.assetId),
                     &srcRect,
                     &dstRect,
                     transform.rotation,
                     nullptr,
                     sprite.flip);
  }
}