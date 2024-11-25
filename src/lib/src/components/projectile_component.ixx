module;

#include <SDL2/SDL.h>

export module projectile_component;

export struct ProjectileComponent {
  bool isFriendly;
  int hitPercentDamage;
  int duration;
  int startTime;

  explicit ProjectileComponent(bool isFriendly = false,
                               int hitPercentDamage = 0,
                               int duration = 0) :
    isFriendly(isFriendly),
    hitPercentDamage(hitPercentDamage),
    duration(duration) {
    this->startTime = SDL_GetTicks();
  }
};