module;

#include "glm/glm.hpp"
#include <string>
#include <utility>
#include <SDL2/SDL.h>

export module text_label_component;


export struct TextLabelComponent {
  glm::vec2 position;
  std::string text;
  std::string assetId;
  SDL_Color color;
  bool isFixed;

  explicit TextLabelComponent(glm::vec2 position = glm::vec2(0.f, 0.f),
                              std::string text = "",
                              std::string assetId = "",
                              const SDL_Color& color = {255, 255, 255, 255},
                              bool isFixed = true)
    : position(position),
      text(std::move(text)),
      assetId(std::move(assetId)),
      color(color),
      isFixed(isFixed) {
  }
};