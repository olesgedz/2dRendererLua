module;
#include <SDL2/SDL.h>

export module systems:animationSystem;
import ecs;
import components;


/// Animation System
/// Probably can name different frames animation by string.

export class AnimationSystem : public System {
public:
  AnimationSystem() {
    requireComponent<SpriteComponent>();
    requireComponent<AnimationComponent>();
  }

  void update(float deltaTime) {
    for (auto entity : getSystemEntities()) {
      auto animation = entity.getComponent<AnimationComponent>();
      auto& sprite = entity.getComponent<SpriteComponent>();

      animation.currentFrame = static_cast<int>((static_cast<size_t>(SDL_GetTicks()) - animation.startTime)
                                                * animation.frameSpeedRate / 1000.0f) % animation.numFrames;
      sprite.srcRect.x = sprite.size.x * animation.currentFrame;
    }
  }
};


