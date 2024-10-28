module;
#define GLM_ENABLE_EXPERIMENTAL
#include <string>
#include <glm//gtx/string_cast.hpp>
#include <memory>
#include <SDL2/SDL_keycode.h>

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
  // requireComponent<KeyboardControlSystem>();
  // requireComponent<SpriteComponent>();
  requireComponent<RigidBodyComponent>();
}

void KeyboardControlSystem::subscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
  eventBus->subscribeToEvent<KeyPressEvent>(this, &KeyboardControlSystem::onKeyPress);
}


void KeyboardControlSystem::onKeyPress(KeyPressEvent& event) {
  Logger::err("NEXXT:");
  auto entities = getSystemEntities();
  // Logger::err("Si11111ze:" + std::to_string(entities.size()));
  // auto entities = getSystemEntities();
  // Logger::err("Size:" + entities.size());
  // for (auto entity : getSystemEntities()) {
  //   // const auto keyboardControl = entity.getComponent<KeyboardControlledComponent>();
  //   auto& sprite = entity.getComponent<SpriteComponent>();
  //   // auto& rigidBody = entity.getComponent<RigidBodyComponent>();
  //   //
  //   // switch (event.keycode) {
  //   //   case SDLK_w:
  //   //     rigidBody.velocity = keyboardControl.upVelocity;
  //   //     sprite.srcRect.y = sprite.size.y * 0;
  //   //     break;
  //   //   case SDLK_d:
  //   //     rigidBody.velocity = keyboardControl.rightVelocity;
  //   //     sprite.srcRect.y = sprite.size.y * 1;
  //   //     break;
  //   //   case SDLK_s:
  //   //     rigidBody.velocity = keyboardControl.downVelocity;
  //   //     sprite.srcRect.y = sprite.size.y * 2;
  //   //     break;
  //   //   case SDLK_a:
  //   //     rigidBody.velocity = keyboardControl.leftVelocity;
  //   //     sprite.srcRect.y = sprite.size.y * 3;
  //   //     break;
  //   //   default:
  //   //     break;
  //   // }
  // }
}

void KeyboardControlSystem::update(float deltaTime) const {
  // for (auto& entity : getSystemEntities()) {
  //   auto& transform = entity.getComponent<TransformComponent>();
  //   const auto& rigidBody = entity.getComponent<RigidBodyComponent>();
  //
  //   transform.position += rigidBody.velocity * deltaTime;
  // }
  auto entities = getSystemEntities();
  Logger::err("Size:" + std::to_string(entities.size()));
}