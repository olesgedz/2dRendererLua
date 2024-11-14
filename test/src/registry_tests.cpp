#include "gtest/gtest.h"

import ecs;

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