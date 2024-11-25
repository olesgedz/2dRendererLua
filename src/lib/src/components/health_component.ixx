
module;

#include <glm/glm.hpp>

export module health_component;

export struct HealthComponent {
  int healthPrecentage;


  explicit HealthComponent(int healthPrecentage = 0) :
    healthPrecentage(healthPrecentage) {
  }
};
