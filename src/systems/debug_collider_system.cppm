module;
#include <SDL2/SDL.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
export module systems:debugColliderSystem;


import components;
import logger;
import ecs;
import assetStorage;

export class DebugColliderSystem : public System {
public:
  DebugColliderSystem();
  void update(SDL_Renderer* renderer) const;

private:
  void _drawCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius) const;
};

DebugColliderSystem::DebugColliderSystem() : System() {
  requireComponent<TransformComponent>();
  // requireComponent<BoxColliderComponent>();
}

void DebugColliderSystem::update(SDL_Renderer* renderer) const {
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
      SDL_RenderDrawPoint(renderer, transform.position.x, transform.position.y);
      SDL_Rect rect = SDL_Rect(transform.position.x + collider.offset.x,
                               transform.position.y + collider.offset.y,
                               collider.size.x * transform.scale.x,
                               collider.size.y * transform.scale.y);
      SDL_RenderDrawRect(renderer, &rect);
    }

    if (entity.hasComponent<CircleColliderComponent>()) {
      const auto& collider = entity.getComponent<CircleColliderComponent>();

      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      _drawCircle(renderer, transform.position.x + collider.offset.x,
                  transform.position.y + collider.offset.y, collider.radius);
    }
  }
}

void DebugColliderSystem::_drawCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius) const {
  const int diameter = (radius * 2);

  int x = (radius - 1);
  int y = 0;
  int tx = 1;
  int ty = 1;
  int error = (tx - diameter);

  while (x >= y) {
    //  Each of the following renders an octant of the circle
    SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

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