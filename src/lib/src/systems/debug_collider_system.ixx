module;
#include <SDL2/SDL.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
export module systems:debug_collider_system;

import components;
import logger;
import ecs;
import asset_storage;

export class DebugColliderSystem : public System {
public:
  DebugColliderSystem();
  void update(SDL_Renderer* renderer, const SDL_Rect& camera) const;

private:
  void _drawCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius, const SDL_Rect& camera) const;
};

DebugColliderSystem::DebugColliderSystem() : System() {
  requireComponent<TransformComponent>();
  requireComponent<BoxColliderComponent>();
}

void DebugColliderSystem::update(SDL_Renderer* renderer, const SDL_Rect& camera) const {
  // It returns a vector of entities that have the required components, so
  // only drawable entities.
  std::vector<Entity> entities = getSystemEntities();

  // We can sort the entities on add Entity or delete
  std::ranges::sort(entities, [](Entity& a, Entity& b) {
    return a.getComponent<SpriteComponent>().zDepth < b.getComponent<SpriteComponent>().zDepth;
  });

  for (auto& entity : entities) {
    const auto& transform = entity.getComponent<TransformComponent>();
    if (entity.hasComponent<BoxColliderComponent>()) {
      const auto& collider = entity.getComponent<BoxColliderComponent>();

      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_Rect rect = SDL_Rect(transform.position.x + collider.offset.x - camera.x,
                               transform.position.y + collider.offset.y - camera.y,
                               collider.size.x * transform.scale.x,
                               collider.size.y * transform.scale.y);
      SDL_RenderDrawRect(renderer, &rect);
    }

    if (entity.hasComponent<CircleColliderComponent>()) {
      const auto& collider = entity.getComponent<CircleColliderComponent>();

      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      _drawCircle(renderer, transform.position.x + collider.offset.x,
                  transform.position.y + collider.offset.y, collider.radius, camera);
    }
  }
}

void DebugColliderSystem::_drawCircle(SDL_Renderer* renderer,
                                      int centreX,
                                      int centreY,
                                      int radius,
                                      const SDL_Rect& camera) const {
  const int diameter = (radius * 2);

  int x = (radius - 1);
  int y = 0;
  int tx = 1;
  int ty = 1;
  int error = (tx - diameter);

  while (x >= y) {
    //  Each of the following renders an octant of the circle
    SDL_RenderDrawPoint(renderer, centreX + x - camera.x, centreY - y - camera.y);
    SDL_RenderDrawPoint(renderer, centreX - x - camera.x, centreY - y - camera.y);
    SDL_RenderDrawPoint(renderer, centreX - x - camera.x, centreY + y - camera.y);
    SDL_RenderDrawPoint(renderer, centreX + y - camera.x, centreY - x - camera.y);
    SDL_RenderDrawPoint(renderer, centreX + y - camera.x, centreY + x - camera.y);
    SDL_RenderDrawPoint(renderer, centreX - y - camera.x, centreY - x - camera.y);
    SDL_RenderDrawPoint(renderer, centreX - y - camera.x, centreY + x - camera.y);

    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0) {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}