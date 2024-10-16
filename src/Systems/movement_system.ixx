module;
#define GLM_ENABLE_EXPERIMENTAL
#include <string>
#include <glm//gtx/string_cast.hpp>

export module movementSystem;

import transformComponent;
import rigidBodyComponent;
import logger;
import ecs;

export class MovementSystem : public System {
public:
  MovementSystem();
  void update(float deltaTime) const;
};

MovementSystem::MovementSystem() : System() {
  requireComponent<TransformComponent>();
  requireComponent<RigidBodyComponent>();
}

void MovementSystem::update(float deltaTime) const {
  for (auto& entity : getSystemEntities()) {
    auto& transform = entity.getComponent<TransformComponent>();
    const auto& rigidBody = entity.getComponent<RigidBodyComponent>();

    transform.position += rigidBody.velocity * deltaTime;

    Logger::log(
        "Updating entity " + std::to_string(entity.getId()) +
        " position " + glm::to_string(transform.position));
  }
}