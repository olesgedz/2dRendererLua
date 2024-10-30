module;

#include <SDL2/SDL.h>

export module systems:camera_movement_system;

import ecs;
import components;
import settings;

export class CameraMovementSystem : public System {
public:
  CameraMovementSystem() {
    requireComponent<CameraFollowComponent>();
    requireComponent<TransformComponent>();
  }

  void update(SDL_Rect& camera) {
    for (auto entity : getSystemEntities()) {
      auto& transform = entity.getComponent<TransformComponent>();
      
      
      if (transform.position.x + (camera.w / 2.f) < Settings::mapSize.x) {
        camera.x = transform.position.x - (Settings::windowWidth / 2.f);
      }
      
      if (transform.position.y + (camera.h / 2.f) < Settings::mapSize.y) {
        camera.y = transform.position.y - (Settings::windowHeight / 2.f);
      }
      
      // Keep the camera inside the window bounds
      camera.x = camera.x < 0 ? 0 : camera.x;
      camera.y = camera.y < 0 ? 0 : camera.y;
      camera.x = camera.x > camera.w ? camera.w : camera.x;
      camera.y = camera.y > camera.h ? camera.h : camera.y;
    }
  }
};