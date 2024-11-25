module;

#include <glm/glm.hpp>

export module box_collider_component;

export struct BoxColliderComponent {
  glm::vec2 size;
  glm::vec2 offset;

  explicit BoxColliderComponent(glm::vec2 size = glm::vec2(0.f, 0.f),
                                glm::vec2 offset = glm::vec2(0.0f, 0.0f)) :
    size(size), offset(offset) {
  }
};