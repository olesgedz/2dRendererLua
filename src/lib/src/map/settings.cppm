module;

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

export module settings;

export class Settings {
 public:
  static int windowWidth;
  static int windowHeight;
  static glm::vec2 mapSize;
};

int Settings::windowWidth;
int Settings::windowHeight;
glm::vec2 Settings::mapSize;