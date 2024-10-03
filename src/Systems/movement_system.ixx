module;

export module movementSystem;

import ecs;

export class MovementSystem : public System {
public:
  MovementSystem();
  void update();
};

MovementSystem::MovementSystem() : System() {
}

void MovementSystem::update() {
  // for (auto& entity : GetEntities()) {
  //
  // }
}