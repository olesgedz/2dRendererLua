#include <deque>
#include <format>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "SDL2/SDL_rect.h"
#include "gtest/gtest.h"

import ecs;
import components;
import systems;
import event_bus;
import settings;
import logger;
import game;

class MovementSystemTest : public ::testing::Test {
 public:
  void SetUp() override {
    registry = std::make_unique<Registry>();
    eventBus = std::make_unique<EventBus>();
    std::cout << "Setup" << std::endl;
  }

  void TearDown() override {
    registry.reset();
    std::cout << "TearDown" << std::endl;
  }

  std::unique_ptr<Registry> registry;
  std::unique_ptr<EventBus> eventBus;
};

TEST_F(MovementSystemTest, RemoveEntitiesOutsideMap) {
  Game::mapWidth = 100;
  Game::mapHeight = 100;

  Entity tank = registry->createEntity();
  registry->addSystem<MovementSystem>();

  tank.addComponent<TransformComponent>(glm::vec2(50.0f, 50.f), glm::vec2(1.0f, 1.0f), 0.f);
  tank.addComponent<RigidBodyComponent>(glm::vec2(20.0f, 0.f));
  tank.addComponent<BoxColliderComponent>(glm::vec2(32.f, 32.f));
  tank.addComponent<HealthComponent>(100);
  tank.group("enemies");
  tank.tag("tank");

  float dt = 0.1f;

  // Systems updates
  for (int i = 0; i < 100; i++) {
    registry->update();
    registry->getSystem<MovementSystem>().update(dt);
  }

  auto entities = registry->getSystem<MovementSystem>().getSystemEntities();

  EXPECT_EQ(entities.size(), 0);
}