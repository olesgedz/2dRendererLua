module;
#define GLM_ENABLE_EXPERIMENTAL
#include <format>
#include <string>
#include <glm//gtx/string_cast.hpp>

export module systems:movement_system;

import components;
import logger;
import ecs;
import game;

export class MovementSystem : public System {
public:
  MovementSystem();
  void update(float deltaTime) const;
};

MovementSystem::MovementSystem() {
  requireComponent<TransformComponent>();
  requireComponent<RigidBodyComponent>();
  requireComponent<KeyboardControlledComponent>(); // cahnge
}

void MovementSystem::update(float deltaTime) const {
  for (auto& entity : getSystemEntities()) {
    auto& transform = entity.getComponent<TransformComponent>();
    auto& rigidBody = entity.getComponent<RigidBodyComponent>();

    transform.position += rigidBody.velocity * deltaTime;
    // rigidBody.velocity = glm::vec2(0.f, 0.f);
    // Logger::err(std::format("Velocity: {}", glm::to_string(rigidBody.velocity)));
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