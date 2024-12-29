module;

#define GLM_ENABLE_EXPERIMENTAL
#include <SDL_timer.h>
#include <memory>
#include <glm//gtx/string_cast.hpp>
#include <SDL2/SDL_keycode.h>

export module systems:projectile_emit_system;

import components;
import logger;
import ecs;
import event_bus;
import events;

export class ProjectileEmitSystem : public System {
public:
  ProjectileEmitSystem();
  void update(std::shared_ptr<Registry> registry) const;
  void subscribeToEvents(std::shared_ptr<EventBus> eventBus);
  void onKeyPress(KeyPressEvent& event);
  void shoot();
};

ProjectileEmitSystem::ProjectileEmitSystem() {
  requireComponent<ProjectileEmitterComponent>();
  requireComponent<TransformComponent>();
}

void ProjectileEmitSystem::update(std::shared_ptr<Registry> registry) const {
  for (auto entity : getSystemEntities()) {
    auto& projectileEmitter = entity.getComponent<ProjectileEmitterComponent>();
    const auto transform = entity.getComponent<TransformComponent>();

    // If emission frequency is zero, bypass re-emission logic
    if (projectileEmitter.repeatFrequency == 0) {
      continue;
    }

    // Check if its time to re-emit a new projectile
    if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency) {
      glm::vec2 projectilePosition = transform.position;
      if (entity.hasComponent<SpriteComponent>()) {
        const auto sprite = entity.getComponent<SpriteComponent>();
        projectilePosition.x += (transform.scale.x * sprite.size.x / 2);
        projectilePosition.y += (transform.scale.y * sprite.size.y / 2);
      }

      // Add a new projectile entity to the registry
      Entity projectile = registry->createEntity();
      projectile.addComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
      projectile.addComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
      projectile.addComponent<SpriteComponent>("bullet-texture", 4, glm::vec2(4, 4));
      // projectile.addComponent<BoxColliderComponent>(4, 4);
      projectile.addComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage,
                                                   projectileEmitter.projectileDuration);
      projectile.group("projectiles");
      projectile.addComponent<BoxColliderComponent>(glm::vec2(4.f, 4.f));

      // Update the projectile emitter component last emission to the current milliseconds
      projectileEmitter.lastEmissionTime = SDL_GetTicks();
    }
  }
}

void ProjectileEmitSystem::subscribeToEvents(std::shared_ptr<EventBus> eventBus) {
  eventBus->subscribeToEvent<KeyPressEvent>(this, &ProjectileEmitSystem::onKeyPress);
}

void ProjectileEmitSystem::onKeyPress(KeyPressEvent& event) {
  switch (event.symbol) {
    case SDLK_SPACE:
      Logger::err("Space pressed");
      shoot();
      break;
    default:
      break;
  }
}

void ProjectileEmitSystem::shoot() {
  for (auto& entity : getSystemEntities()) {
    //GetPlayer
    if (entity.hasComponent<CameraFollowComponent>()) {
      auto& transform = entity.getComponent<TransformComponent>();
      auto& rigidBody = entity.getComponent<RigidBodyComponent>();
      auto& projectileEmitter = entity.getComponent<ProjectileEmitterComponent>();
      glm::vec2 projectilePosition = transform.position;
      if (entity.hasComponent<SpriteComponent>()) {
        auto sprite = entity.getComponent<SpriteComponent>();

        projectilePosition.x += (transform.scale.x * sprite.size.x / 2.f);
        projectilePosition.y += (transform.scale.y * sprite.size.y / 2.f);
      }
      glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;

      int directionX = 0;
      int directionY = 0;
      if (rigidBody.velocity.x > 0) directionX = +1;
      if (rigidBody.velocity.x < 0) directionX = -1;
      if (rigidBody.velocity.y > 0) directionY = +1;
      if (rigidBody.velocity.y < 0) directionY = -1;
      projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
      projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;

      Entity projectile = entity.registry->createEntity();
      projectile.addComponent<TransformComponent>(projectilePosition, transform.scale, transform.rotation);
      projectile.addComponent<RigidBodyComponent>(projectileVelocity);
      projectile.addComponent<SpriteComponent>("bullet-texture", 4, glm::vec2(4, 4));
      projectile.addComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage,
                                                   10000);
      projectile.addComponent<BoxColliderComponent>(glm::vec2(4.f, 4.f));
      projectile.group("projectiles");
      projectileEmitter.lastEmissionTime = SDL_GetTicks();
    }
  }
}