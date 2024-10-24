module;

#include <glm/glm.hpp>

export module components:rigidBodyComponent;

export struct RigidBodyComponent {
  glm::vec2 velocity;
  explicit RigidBodyComponent(glm::vec2 velocity = glm::vec2(0.f, 0.f)) : velocity(velocity) {
  }
};