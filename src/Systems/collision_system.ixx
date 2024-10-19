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

    // interested in
    for (auto i = entities.begin(); i != entities.end(); i++) {
      Entity compared = *i;

      auto colliderCompared = compared.getComponent<BoxColliderComponent>();
      auto transformCompared = compared.getComponent<TransformComponent>();
      Collider dataCompared = {.boxCollider = colliderCompared, .transform = transformCompared};

      //need to be checked
      for (auto j = i; j != entities.end(); j++) {
        Entity other = *j;

        if (compared == other)
          continue;
        auto colliderOther = other.getComponent<BoxColliderComponent>();
        auto transformOther = other.getComponent<TransformComponent>();

        Collider dataOther = {.boxCollider = colliderOther, .transform = transformOther};

        if (_checkAABBCollision(dataCompared, dataOther)) {
          Logger::log(std::to_string(compared.getId()) + " collided with " + std::to_string(other.getId()));
        }
      }
    }
  }

private:
  bool _checkAABBCollision(Collider& compared, Collider& other) {
    bool collided = false;
    glm::vec2 positionCompared = compared.transform.position + compared.boxCollider.offset;
    glm::vec2 positionOther = other.transform.position + other.boxCollider.offset;

    if (positionCompared.x < positionOther.x + other.boxCollider.size.x * other.transform.scale.x
        &&
        positionCompared.x + compared.boxCollider.size.x * compared.transform.scale.x > positionOther.x &&
        positionCompared.y < positionOther.y + other.boxCollider.size.y * other.transform.scale.y
        &&
        positionCompared.y + compared.boxCollider.size.y * compared.transform.scale.y > positionOther.y
    )
      collided = true;
    return collided;
  }
};
