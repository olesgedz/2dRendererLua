module;

#include <SDL_timer.h>

export module systems:script_system;

import ecs;
import components;
import event_bus;
import events;
import logger;

export class ScriptSystem : public System {
public:
  ScriptSystem() {
    requireComponent<ScriptComponent>();
  }

  void update() {
    for (auto& entity : getSystemEntities()) {
      const auto scriptComponent = entity.getComponent<ScriptComponent>();

      scriptComponent.func();
    }
  }
};