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