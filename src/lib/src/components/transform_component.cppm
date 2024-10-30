module;

#include <glm/glm.hpp>

export module components:transform_component;

export struct TransformComponent {
  glm::vec2 position{};
  glm::vec2 scale{};
  double rotation;

  // TransformComponent() : position(glm::vec2(0.0f)), scale(glm::vec2(1.0f)), rotation(0.0) {}
  explicit TransformComponent(glm::vec2 position = glm::vec2(0.f, 0.f), glm::vec2 scale = glm::vec2(0.f, 0.f), double rotation = 0.0) {
        this->position = position;
        this->scale = scale;
        this->rotation = rotation;
  }
};