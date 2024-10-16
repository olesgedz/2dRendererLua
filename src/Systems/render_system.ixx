module;
#include <SDL2/SDL.h>

export module renderSystem;


import transformComponent;
import rigidBodyComponent;
import logger;
import ecs;
export import spriteComponent;

export class RenderSystem : public System {
public:
  RenderSystem();
  void update(SDL_Renderer* renderer) const;
};

RenderSystem::RenderSystem() : System() {
  requireComponent<TransformComponent>();
  requireComponent<RigidBodyComponent>();
}

void RenderSystem::update(SDL_Renderer* renderer) const {
  for (auto& entity : getSystemEntities()) {
    const auto& transform = entity.getComponent<TransformComponent>();
    const auto& sprite = entity.getComponent<SpriteComponent>();

    SDL_Rect objRect = {
        static_cast<int>(transform.position.x),
        static_cast<int>(transform.position.y),
        static_cast<int>(sprite.size.x),
        static_cast<int>(sprite.size.y)
    };
    SDL_SetRenderDrawColor(renderer, sprite.color.r, sprite.color.g, sprite.color.b, sprite.color.a);
    SDL_RenderFillRect(renderer, &objRect);
  }
}