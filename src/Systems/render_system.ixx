module;
#include <SDL2/SDL.h>
#include <memory>
export module renderSystem;


import transformComponent;
import rigidBodyComponent;
import logger;
import ecs;
import assetStorage;
export import spriteComponent;

export class RenderSystem : public System {
public:
  RenderSystem();
  void update(SDL_Renderer* renderer, std::unique_ptr<AssetStorage>& assetStorage) const;
};

RenderSystem::RenderSystem() : System() {
  requireComponent<TransformComponent>();
  requireComponent<SpriteComponent>();
}

void RenderSystem::update(SDL_Renderer* renderer, std::unique_ptr<AssetStorage>& assetStorage) const {
  for (auto& entity : getSystemEntities()) {
    const auto& transform = entity.getComponent<TransformComponent>();
    const auto& sprite = entity.getComponent<SpriteComponent>();

    // SDL_Rect objRect = {
    //     static_cast<int>(transform.position.x),
    //     static_cast<int>(transform.position.y),
    //     static_cast<int>(sprite.size.x),
    //     static_cast<int>(sprite.size.y)
    // };
    // SDL_SetRenderDrawColor(renderer, sprite.color.r, sprite.color.g, sprite.color.b, sprite.color.a);
    // SDL_RenderFillRect(renderer, &objRect);
    SDL_Rect srcRect = sprite.srcRect;

    SDL_Rect dstRect = {
        static_cast<int>(transform.position.x),
        static_cast<int>(transform.position.y),
        static_cast<int>(sprite.size.x * transform.scale.x),
        static_cast<int>(sprite.size.y * transform.scale.y)
    };

    SDL_RenderCopyEx(renderer, assetStorage->getTexture(sprite.assetId),
                     &srcRect, &dstRect, transform.rotation, nullptr, SDL_FLIP_NONE);
  }
}