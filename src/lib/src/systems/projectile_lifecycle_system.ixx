module;

#include <SDL_timer.h>

export module projectile_lifecycle_system;

import ecs;
import components;
import event_bus;
import events;
import logger;

export class ProjectileLifecycleSystem : public System {
public:
  ProjectileLifecycleSystem() {
    requireComponent<ProjectileComponent>();
  }

  void update() {
    for (auto& entity : getSystemEntities()) {
      auto& projectile = entity.getComponent<ProjectileComponent>();

      if (SDL_GetTicks() - projectile.startTime > projectile.duration) {
        entity.kill();
      }
    }
  }
};