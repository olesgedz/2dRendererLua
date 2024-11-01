module;

#include <cmath>
#include <memory>
#include <string>

export module systems:damage_system;

import ecs;
import components;
import event_bus;
import events;
import logger;

export class DamageSystem : public System {
public:
  DamageSystem() {
    requireComponent<BoxColliderComponent>();
  }

  void subscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
    eventBus->subscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
  }

  void onCollision(CollisionEvent& event) {
    Logger::log(
        "Damage system received an event Collision between entites: " + std::to_string(event.a.getId()) + " and " +
        std::to_string(event.b.getId()));
    // event.a.kill();
    // event.b.kill();
  }

  void update() {
  }
};