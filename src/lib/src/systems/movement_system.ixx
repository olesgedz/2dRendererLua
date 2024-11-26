module;
#define GLM_ENABLE_EXPERIMENTAL
#include <string>
#include <glm//gtx/string_cast.hpp>

export module systems:movement_system;

import components;
import logger;
import ecs;

export class MovementSystem : public System {
public:
  MovementSystem();
  void update(float deltaTime) const;
};

MovementSystem::MovementSystem() {
  requireComponent<TransformComponent>();
  requireComponent<RigidBodyComponent>();
}

void MovementSystem::update(float deltaTime) const {
  for (auto& entity : getSystemEntities()) {
    auto& transform = entity.getComponent<TransformComponent>();
    const auto& rigidBody = entity.getComponent<RigidBodyComponent>();

    transform.position += rigidBody.velocity * deltaTime;
  }
}