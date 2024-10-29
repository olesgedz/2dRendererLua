module;

#include <SDL2/SDL.h>

export module systems:camera_movement_system;

import ecs;
import components;

export class CameraMovementSystem : public System {
public:
  CameraMovementSystem() {
    requireComponent<CameraFollowComponent>();
    requireComponent<TransformComponent>();
  }

  void update(SDL_Rect& camera) {
    for (auto entity : getSystemEntities()) {
      auto& transform = entity.getComponent<TransformComponent>();

      camera.x = transform.position.x;
      camera.y = transform.position.y;
    }
  }
};