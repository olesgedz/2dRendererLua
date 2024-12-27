module;
#define GLM_ENABLE_EXPERIMENTAL
#include <format>
#include <string>
#include <glm//gtx/string_cast.hpp>
#include <SDL2/SDL_render.h>

export module systems:movement_system;

import components;
import logger;
import ecs;
import game;
import event_bus;
import collision_event;

export class MovementSystem : public System {
public:
  MovementSystem();
  void update(float deltaTime) const;

  void subscribeToEvents(const std::shared_ptr<EventBus> eventBus);
  void onCollision(CollisionEvent& event);

private:
  void _onEnemyHitObstacle(Entity& enemy, Entity& obstacle);
};

MovementSystem::MovementSystem() {
  requireComponent<TransformComponent>();
  requireComponent<RigidBodyComponent>();
}

void MovementSystem::subscribeToEvents(const std::shared_ptr<EventBus> eventBus) {
  eventBus->subscribeToEvent<CollisionEvent>(this, &MovementSystem::onCollision);
}

void MovementSystem::onCollision(CollisionEvent& event) {
  Entity a = event.a;
  Entity b = event.b;

  if (a.belongsToGroup("obstacles") && b.belongsToGroup("enemies")) {
    _onEnemyHitObstacle(a, b);
  }

  if (b.belongsToGroup("obstacles") && a.belongsToGroup("enemies")) {
    _onEnemyHitObstacle(a, b);
  }
}

void MovementSystem::_onEnemyHitObstacle(Entity& enemy, Entity& obstacle) {
  if (enemy.hasComponent<RigidBodyComponent>() && obstacle.hasComponent<SpriteComponent>()) {
    auto& rigidBody = enemy.getComponent<RigidBodyComponent>();
    auto& sprite = enemy.getComponent<SpriteComponent>();

    if (rigidBody.velocity.x != 0) {
      rigidBody.velocity.x = -rigidBody.velocity.x;
      sprite.flip = (sprite.flip == SDL_FLIP_NONE) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    }

    if (rigidBody.velocity.y != 0) {
      rigidBody.velocity.y = -rigidBody.velocity.y;
      sprite.flip = (sprite.flip == SDL_FLIP_NONE) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
    }
  }
}

void MovementSystem::update(float deltaTime) const {
  for (auto& entity : getSystemEntities()) {
    auto& transform = entity.getComponent<TransformComponent>();
    auto& rigidBody = entity.getComponent<RigidBodyComponent>();

    transform.position += rigidBody.velocity * deltaTime;

    if (entity.hasTag("player")) {
      int paddingLeft = 10;
      int paddingTop = 10;
      int paddingRight = 50;
      int paddingBottom = 100;

      transform.position.x = transform.position.x < paddingLeft ? paddingLeft : transform.position.x;
      transform.position.x = transform.position.x > Game::mapWidth - paddingRight
                               ? Game::mapWidth - paddingRight
                               : transform.position.x;
      transform.position.y = transform.position.y < paddingTop ? paddingTop : transform.position.y;
      transform.position.y = transform.position.y > Game::mapHeight - paddingBottom
                               ? Game::mapHeight - paddingBottom
                               : transform.position.y;
    }

    bool outSideTheMap = false;
    if (transform.position.x < 0 ||
        transform.position.x > static_cast<float>(Game::mapWidth) ||
        transform.rotation < 0 ||
        transform.position.y > static_cast<float>(Game::mapHeight)) {
      outSideTheMap = true;
    }

    if (outSideTheMap && !entity.hasTag("player")) {
      entity.kill();
    }
  }
}