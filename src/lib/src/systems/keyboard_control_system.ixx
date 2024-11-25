module;
#define GLM_ENABLE_EXPERIMENTAL
#include <string>
#include <glm//gtx/string_cast.hpp>
#include <memory>
#include <SDL2/SDL_keycode.h>

export module keyboard_control_system;

import components;
import logger;
import ecs;
import event_bus;
import events;

export class KeyboardControlSystem : public System {
public:
  KeyboardControlSystem() {
    requireComponent<SpriteComponent>();
    requireComponent<RigidBodyComponent>();
    requireComponent<KeyboardControlledComponent>();
  }

  void update() const {
  }

  void subscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
    eventBus->subscribeToEvent<KeyPressEvent>(this, &KeyboardControlSystem::onKeyPress);
  }

  void onKeyPress(KeyPressEvent& event) {
    for (auto entity : getSystemEntities()) {
      const auto keyboardControl = entity.getComponent<KeyboardControlledComponent>();
      auto& sprite = entity.getComponent<SpriteComponent>();
      auto& rigidBody = entity.getComponent<RigidBodyComponent>();

      switch (event.symbol) {
        case SDLK_w:
          rigidBody.velocity = keyboardControl.upVelocity;
          sprite.srcRect.y = sprite.size.y * 0;
          break;
        case SDLK_d:
          rigidBody.velocity = keyboardControl.rightVelocity;
          sprite.srcRect.y = sprite.size.y * 1;
          break;
        case SDLK_s:
          rigidBody.velocity = keyboardControl.downVelocity;
          sprite.srcRect.y = sprite.size.y * 2;
          break;
        case SDLK_a:
          rigidBody.velocity = keyboardControl.leftVelocity;
          sprite.srcRect.y = sprite.size.y * 3;
          break;
        default:
          break;
      }
    }
  }
};