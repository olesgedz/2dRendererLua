module;

#include <glm/glm.hpp>

export module TransformComponent;

struct TransformComponent {
  glm::vec2 position;
  glm::vec2 scale;
  double rotation;
};