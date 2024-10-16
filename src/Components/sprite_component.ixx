module;

#include "glm/glm.hpp"

export module spriteComponent;

export struct SpriteComponent {
  glm::vec2 size;
  glm::vec4 color;

  explicit SpriteComponent(glm::vec2 size = glm::vec2(0.f, 0.f),
                           glm::vec4 color = glm::vec4(255.f, 255.f, 255.f, 255.f)) : size(size), color(color) {
  }
};