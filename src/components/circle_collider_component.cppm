module;

#include <glm/glm.hpp>

export module components:circleColliderComponent;

export struct CircleColliderComponent {
  float radius;
  glm::vec2 offset;

  explicit CircleColliderComponent(float radius = 0,
                                   glm::vec2 offset = glm::vec2(0.0f, 0.0f)) :
    radius(radius), offset(offset) {
  }
};
