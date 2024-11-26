#include <gtest/gtest.h>

import pool;
import components;

template <typename T>
struct PoolTest {
  std::unordered_map<int, int>& _entityIdToIndex;
  std::unordered_map<int, int>& _indexToEntityId;
  std::vector<T>& _data;
  int& _size;

  PoolTest(Pool<T>& pool)
      : _entityIdToIndex(pool._entityIdToIndex),
        _indexToEntityId(pool._indexToEntityId),
        _data(pool._data),
        _size(pool._size) {}
};

TEST(Pool, isEmpty) {
  auto pool = std::make_shared<Pool<HealthComponent>>();
  PoolTest<HealthComponent> poolTest(*pool);

  EXPECT_EQ(pool->getSize(), 0);
  EXPECT_EQ(poolTest._size, 0);

  int entityId = 0;
  pool->set(entityId, HealthComponent(100));
  EXPECT_NE(pool->getSize(), 0);
  EXPECT_NE(poolTest._size, 0);
}

TEST(Pool, AddEntity) {
  auto pool = std::make_shared<Pool<HealthComponent>>(1);

  EXPECT_EQ(pool->getSize(), 0);

  int entityId = 0;
  pool->set(entityId++, HealthComponent(100));
  pool->set(entityId++, HealthComponent(100));
  pool->set(entityId, HealthComponent(100));
  EXPECT_EQ(pool->getSize(), 3);

  pool->removeFromPull(entityId);
  EXPECT_EQ(pool->getSize(), 2);

}

TEST(Pool, RemoveEntity) {
  auto pool = std::make_shared<Pool<HealthComponent>>();
  PoolTest<HealthComponent> poolTest(*pool);

  int entityId = 0;
  int entityId2 = 1;

  pool->set(entityId, HealthComponent(100));
  pool->set(entityId2, HealthComponent(102));

  EXPECT_EQ(pool->getSize(), 2);
  EXPECT_EQ(poolTest._entityIdToIndex[entityId2], 1);
  EXPECT_EQ(poolTest._entityIdToIndex[entityId], 0);

  pool->removeFromPull(entityId2);
  EXPECT_EQ(poolTest._entityIdToIndex[entityId2], 0);
}

TEST(Pool, SetEntity) {
  auto pool = std::make_shared<Pool<HealthComponent>>();
  PoolTest<HealthComponent> poolTest(*pool);

  int entityId = 0;

  pool->set(entityId, HealthComponent(100));

  EXPECT_EQ(pool->getSize(), 1);
  EXPECT_EQ(pool->get(entityId).healthPercentage, 100);

  pool->set(entityId, HealthComponent(42));
  EXPECT_EQ(pool->getSize(), 1);
  EXPECT_EQ(pool->get(entityId).healthPercentage, 42);
}