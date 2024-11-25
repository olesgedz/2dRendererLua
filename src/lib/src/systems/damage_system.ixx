module;

#include <cmath>
#include <memory>
#include <string>

export module damage_system;

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
    Entity a = event.a;
    Entity b = event.b;

    if (a.belongsToGroup("projectiles") && b.hasTag("player")) {
      onProjectileHitsPlayer(a, b);
    }

    if (b.belongsToGroup("projectiles") && a.hasTag("player")) {
      onProjectileHitsPlayer(b, a);
    }

    if (a.belongsToGroup("projectiles") && b.belongsToGroup("enemies")) {
      onProjectileHitsEnemy(a, b);
    }

    if (b.belongsToGroup("projectiles") && a.belongsToGroup("enemies")) {
      onProjectileHitsEnemy(b, a);
    }
  }

  void update() {
  }

  void onProjectileHitsPlayer(Entity projectile, Entity player) {
    const auto projectileComponent = projectile.getComponent<ProjectileComponent>();
    auto& healthComponent = player.getComponent<HealthComponent>();

    if (!projectileComponent.isFriendly) {
      healthComponent.healthPercentage -= projectileComponent.hitPercentDamage;
      Logger::log("Player health: " + std::to_string(healthComponent.healthPercentage));
      projectile.kill();
    }
    if (healthComponent.healthPercentage <= 0) {
      player.kill();
    }
  }

  void onProjectileHitsEnemy(Entity projectile, Entity enemy) {
    const auto projectileComponent = projectile.getComponent<ProjectileComponent>();
    auto& healthComponent = enemy.getComponent<HealthComponent>();

    if (projectileComponent.isFriendly) {
      healthComponent.healthPercentage -= projectileComponent.hitPercentDamage;
      Logger::log("Enemy health: " + std::to_string(healthComponent.healthPercentage));
      projectile.kill();
    }
    if (healthComponent.healthPercentage <= 0) {
      // enemy.kill();
    }
  }
};