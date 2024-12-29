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

class DamageSystemTest : public ::testing::Test {
 public:
  void SetUp() override {
    registry = std::make_shared<Registry>();
    eventBus = std::make_shared<EventBus>();
    std::cout << "Setup" << std::endl;
  }

  void TearDown() override {
    registry.reset();
    std::cout << "TearDown" << std::endl;
  }

  std::shared_ptr<Registry> registry;
  std::shared_ptr<EventBus> eventBus;
};

TEST_F(DamageSystemTest, AddComponent) {
  // Added because of functionality of entity culling
  Game::mapWidth = 1000;
  Game::mapHeight = 1000;

  Entity tank = registry->createEntity();
  registry->addSystem<MovementSystem>();
  registry->addSystem<DamageSystem>();
  registry->addSystem<CollisionSystem>();

  tank.addComponent<TransformComponent>(glm::vec2(300.0f, 10.f), glm::vec2(1.0f, 1.0f), 0.f);
  tank.addComponent<RigidBodyComponent>(glm::vec2(0.0f, 0.f));
  tank.addComponent<BoxColliderComponent>(glm::vec2(32.f, 32.f));
  tank.addComponent<HealthComponent>(100);
  tank.group("enemies");
  tank.tag("tank");

  Entity truck = registry->createEntity();

  truck.addComponent<TransformComponent>(glm::vec2(10.0f, 10.f), glm::vec2(1.0f, 1.0f), 0.f);
  truck.addComponent<RigidBodyComponent>(glm::vec2(0.0f, 0.f));
  truck.addComponent<BoxColliderComponent>(glm::vec2(32.f, 32.f));
  truck.addComponent<HealthComponent>(100);
  truck.group("enemies");
  truck.tag("truck");

  Entity projectile = registry->createEntity();
  auto tank_transform = truck.getComponent<TransformComponent>();
  projectile.addComponent<TransformComponent>(tank_transform.position + glm::vec2(0.f, 10.f), tank_transform.scale,
                                              tank_transform.rotation);
  projectile.addComponent<RigidBodyComponent>(glm::vec2(150.f, 150.f));
  projectile.addComponent<ProjectileComponent>(true, 100, 10000);
  projectile.addComponent<BoxColliderComponent>(glm::vec2(4.f, 4.f));
  projectile.group("projectiles");

  eventBus->reset();

  // Subscriptions
  registry->getSystem<DamageSystem>().subscribeToEvents(eventBus);

  // Update the registry to process the entities to be added or killed

  float dt = 0.1f;
  SDL_Rect camera;
  camera.x = 0;
  camera.y = 0;
  camera.w = Settings::windowWidth;
  camera.h = Settings::windowHeight;

  // Systems updates
  for (int i = 0; i < 2; i++) {
    registry->update();
    registry->getSystem<MovementSystem>().update(dt);
    registry->getSystem<CollisionSystem>().update(eventBus);
    registry->getSystem<DamageSystem>().update();
  }

  auto entities = registry->getSystem<MovementSystem>().getSystemEntities();

  EXPECT_EQ(entities.size(), 1);
  EXPECT_EQ(entities[0].getTag(), "tank");
}