module;

#include <string>
#include <glm/glm.hpp>

export module collisionSystem;

import ecs;
import logger;

import boxColliderComponent;
import transformComponent;
import rigidBodyComponent;


export class CollisionSystem : public System {
public:
  CollisionSystem() {
    requireComponent<BoxColliderComponent>();
    requireComponent<TransformComponent>();
  }

  struct Collider {
    BoxColliderComponent boxCollider;
    TransformComponent transform;
  };

  void update() {
    auto entities = getSystemEntities();
    for (auto entity : entities) {
      const auto& boxCollider = entity.getComponent<BoxColliderComponent>();
      const auto& transform = entity.getComponent<TransformComponent>();
      Collider comparedCollider = {.boxCollider = boxCollider, .transform = transform};
      for (auto other : entities) {
        if (other.getId() == entity.getId())
          continue;

        const auto& boxColliderOther = other.getComponent<BoxColliderComponent>();
        const auto& transformOther = other.getComponent<TransformComponent>();

        Collider otherCollider = {.boxCollider = boxColliderOther, .transform = transformOther};

        if (isCollided(comparedCollider, otherCollider)) {
          Logger::log(std::to_string(entity.getId()) + " collided with " + std::to_string(other.getId()));
          if (entity.hasComponent<RigidBodyComponent>()) {
            auto& rigidBody = entity.getComponent<RigidBodyComponent>();
            rigidBody.velocity = glm::vec2(0, rigidBody.velocity.y);
          }
          if (other.hasComponent<RigidBodyComponent>()) {
            auto& rigidBody = other.getComponent<RigidBodyComponent>();
            rigidBody.velocity = glm::vec2(0, rigidBody.velocity.y);
          }
        }
      }
    }
  }

private:
  bool isCollided(Collider& compared, Collider& other) {
    bool collided = false;
    if (compared.transform.position.x < other.transform.position.x + other.boxCollider.size.x * other.transform.scale.x
        &&
        compared.transform.position.x + compared.boxCollider.size.x * compared.transform.scale.x > other.transform.
        position.x &&
        compared.transform.position.y < other.transform.position.y + other.boxCollider.size.y * other.transform.scale.y
        &&
        compared.transform.position.y + compared.boxCollider.size.y * compared.transform.scale.y > other.transform.
        position.y
    )
      collided = true;
    return collided;
  }
};
