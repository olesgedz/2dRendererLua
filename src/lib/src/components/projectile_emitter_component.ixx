module;

#include <SDL_timer.h>
#include <glm/glm.hpp>

export module projectile_emitter_component;


export struct ProjectileEmitterComponent {
  glm::vec2 projectileVelocity;
  float repeatFrequency;
  float projectileDuration;
  float hitPercentDamage;
  float lastEmissionTime;
  bool isFriendly;

  explicit ProjectileEmitterComponent(glm::vec2 projectileVelocity = glm::vec2(0.f),
                                      float repeatFrequency = 0.f,
                                      float projectileDuration = 0.f,
                                      float hitPercentDamage = 0.f,
                                      bool isFriendly = false) :
    projectileVelocity(projectileVelocity),
    repeatFrequency(repeatFrequency),
    projectileDuration(projectileDuration), hitPercentDamage(hitPercentDamage),
    isFriendly(isFriendly) {
    this->lastEmissionTime = SDL_GetTicks();
  }
};