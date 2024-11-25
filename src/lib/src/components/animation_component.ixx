module;

#include "glm/glm.hpp"
#include <string>
#include <utility>
#include <SDL2/SDL.h>

export module animation_component;


export struct AnimationComponent {
  int numFrames;
  int currentFrame = 1;;
  int frameSpeedRate;
  bool isLoop;
  size_t startTime = 0;

  explicit AnimationComponent(int numFrames = 1,
                              int frameSpeedRate = 1,
                              bool isLoop = true)
    : numFrames(numFrames), frameSpeedRate(frameSpeedRate), isLoop(isLoop), startTime(SDL_GetTicks()) {
  }
};