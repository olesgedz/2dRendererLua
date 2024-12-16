module;

#include "glm/glm.hpp"
#include <string>
#include <utility>
#include <SDL2/SDL.h>

export module sprite_component;

export struct SpriteComponent {
  glm::vec2 size;
  glm::vec4 color;
  std::string assetId;
  SDL_Rect srcRect{};
  int zDepth;
  bool isFixed;
  SDL_RendererFlip flip;

  explicit SpriteComponent(std::string assetId = "",
                           int zDepth = 0,
                           glm::vec2 size = glm::vec2(0.f, 0.f),
                           glm::vec4 color = glm::vec4(255.f, 255.f, 255.f, 255.f),
                           bool isFixed = false,
                           glm::vec2 src = glm::vec2(0.f, 0.f)
      )
    : assetId(std::move(assetId)), size(size), color(color), zDepth(zDepth), isFixed(isFixed) {
    srcRect = {static_cast<int>(src.x), static_cast<int>(src.y), static_cast<int>(size.x), static_cast<int>(size.y)};
    flip = SDL_FLIP_NONE;
  }
};