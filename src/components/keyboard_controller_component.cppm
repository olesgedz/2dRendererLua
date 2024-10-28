module;

#include <glm/glm.hpp>

export module components:keyboard_controlled_component;


export struct KeyboardControlledComponent {
  glm::vec2 upVelocity;
  glm::vec2 rightVelocity;
  glm::vec2 downVelocity;
  glm::vec2 leftVelocity;

  explicit KeyboardControlledComponent(glm::vec2 upVelocity = glm::vec2(0),
                                       glm::vec2 rightVelocity = glm::vec2(0),
                                       glm::vec2 downVelocity = glm::vec2(0),
                                       glm::vec2 leftVelocity = glm::vec2(0)) :
    upVelocity(upVelocity),
    rightVelocity(rightVelocity),
    downVelocity(downVelocity),
    leftVelocity(leftVelocity) {
  }
};
