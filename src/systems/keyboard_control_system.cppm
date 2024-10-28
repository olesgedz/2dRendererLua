module;
#define GLM_ENABLE_EXPERIMENTAL
#include <string>
#include <glm//gtx/string_cast.hpp>
#include <memory>

export module systems:keyboard_control_system;

import components;
import logger;
import ecs;
import event_bus;
import events;

export class KeyboardControlSystem : public System {
public:
  KeyboardControlSystem();
  void update(float deltaTime) const;
  void subscribeToEvents(std::unique_ptr<EventBus>& eventBus);

  void onKeyPress(KeyPressEvent& event);
};

KeyboardControlSystem::KeyboardControlSystem() : System() {
  requireComponent<TransformComponent>();
  requireComponent<RigidBodyComponent>();
}

void KeyboardControlSystem::subscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
  eventBus->subscribeToEvent<KeyPressEvent>(this, &KeyboardControlSystem::onKeyPress);
}


void KeyboardControlSystem::onKeyPress(KeyPressEvent& event) {
  std::string keyCode = std::to_string(event.keycode);
  std::string keySymbol(1, event.keycode);
  Logger::log("OnKeyPress event keycode: " + keyCode + " Symbol: " + keySymbol);
}

void KeyboardControlSystem::update(float deltaTime) const {
  // for (auto& entity : getSystemEntities()) {
  //   auto& transform = entity.getComponent<TransformComponent>();
  //   const auto& rigidBody = entity.getComponent<RigidBodyComponent>();
  //
  //   transform.position += rigidBody.velocity * deltaTime;
  // }
}