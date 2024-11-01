module;

#define GLM_ENABLE_EXPERIMENTAL
#include <SDL_timer.h>
#include <memory>
#include <glm//gtx/string_cast.hpp>

export module systems:projectile_emit_system;


import components;
import logger;
import ecs;

export class ProjectileEmitSystem : public System {
public:
  ProjectileEmitSystem();
  void update(std::unique_ptr<Registry>& registry) const;
};

ProjectileEmitSystem::ProjectileEmitSystem() : System() {
  requireComponent<ProjectileEmitterComponent>();
  requireComponent<TransformComponent>();
}

void ProjectileEmitSystem::update(std::unique_ptr<Registry>& registry) const {
  for (auto& entity : getSystemEntities()) {
    auto& projectileEmitter = entity.getComponent<ProjectileEmitterComponent>();
    const auto transform = entity.getComponent<TransformComponent>();

    if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency) {
      // add new projectile

      glm::vec2 projectilePosition = transform.position;
      if (entity.hasComponent<SpriteComponent>()) {
        const auto sprite = entity.getComponent<SpriteComponent>();
        projectilePosition += transform.scale * sprite.size / 2.f;
      }
      auto projectile = registry->createEntity();
      projectile.addComponent<TransformComponent>(projectilePosition, glm::vec2(1.f, 1.f), transform.rotation);
      projectile.addComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
      projectile.addComponent<SpriteComponent>("bullet-image", 4, glm::vec2(4, 4));
      projectile.addComponent<BoxColliderComponent>(glm::vec2(4, 4));

      projectileEmitter.lastEmissionTime = SDL_GetTicks();
    }
  }
}