#include <deque>
#include <glm/gtc/constants.hpp>

#include "gtest/gtest.h"

import ecs;
import components;

class RegistryTest : public ::testing::Test {
 public:
  void SetUp() override {
    registry = std::make_unique<Registry>();
    std::cout << "Setup" << std::endl;
  }

  void TearDown() override {
    registry.reset();
    std::cout << "TearDown" << std::endl;
  }

  std::unique_ptr<Registry> registry;
};

TEST_F(RegistryTest, AddComponent) {
  auto entity = registry->createEntity();
  entity.addComponent<HealthComponent>(100);
  EXPECT_EQ(entity.hasComponent<HealthComponent>(), true);
}

TEST_F(RegistryTest, RemoveComponent) {
  auto entity = registry->createEntity();
  entity.addComponent<HealthComponent>(100);
  EXPECT_EQ(entity.hasComponent<HealthComponent>(), true);
  entity.removeComponent<HealthComponent>();
  EXPECT_EQ(entity.hasComponent<HealthComponent>(), false);
}

TEST(Registry, AddEntity) {
  auto registry = std::make_unique<Registry>();
  auto entity = registry->createEntity();
  EXPECT_EQ(entity.getId(), 0);
  EXPECT_EQ(registry->getNumEntities(), 1);
  registry->update();
  EXPECT_EQ(registry->getNumEntities(), 1);
}

TEST(Registry, RemoveEntity) {
  // Add entity
  auto registry = std::make_unique<Registry>();
  auto entity = registry->createEntity();
  EXPECT_EQ(entity.getId(), 0);
  EXPECT_EQ(registry->getNumEntities(), 1);
  registry->update();
  EXPECT_EQ(registry->getNumEntities(), 1);

  // Remove entity
  entity.kill();
  EXPECT_EQ(registry->getNumEntities(), 1);
  registry->update();
  EXPECT_EQ(registry->getNumEntities(), 1);
}

TEST(Registry, AddTag) {
  // Add entity
  auto registry = std::make_unique<Registry>();
  auto entity = registry->createEntity();
  entity.tag("test_tag");
  EXPECT_EQ(entity.hasTag("test_tag"), true);
}

TEST(Registry, HasATagNoTagError) {
  // Add entity
  auto registry = std::make_unique<Registry>();
  auto entity = registry->createEntity();
  entity.tag("test_tag");
  EXPECT_EQ(entity.hasTag("test_tag"), true);
  EXPECT_EQ(entity.hasTag("test_tag_another"), false);
}

TEST(Registry, RemoveTag) {
  // Add entity
  auto registry = std::make_unique<Registry>();
  auto entity = registry->createEntity();
  entity.tag("test_tag");
  EXPECT_EQ(entity.hasTag("test_tag"), true);

  entity.registry->removeEntityTag(entity);
  EXPECT_EQ(entity.hasTag("test_tag"), false);
}

TEST(Registry, AddGroup) {
  // Add entity
  auto registry = std::make_unique<Registry>();
  auto entity = registry->createEntity();
  entity.group("test_group");

  EXPECT_EQ(entity.belongsToGroup("test_group"), true);
}