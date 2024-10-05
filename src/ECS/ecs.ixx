module;

#include <bitset>
#include <memory>
#include <set>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <algorithm>

export module ecs;
import pool;
import logger;

constexpr unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct BaseComponent {
 protected:
  static int _nextId;
};

int BaseComponent::_nextId = 0;

template <typename T>
class Component : public BaseComponent {
  static int getId() {
    static int id = _nextId++;
    return id;
  }
};

class Entity {
 public:
  Entity(int id) : _id(id) {};

  Entity(const Entity& other) = default;

  Entity& operator=(const Entity& other) = default;

  int getId() const { return _id; }

  bool operator==(const Entity& other) const { return _id == other._id; }
  bool operator>(const Entity& other) const { return _id > other._id; }
  bool operator<(const Entity& other) const { return _id < other._id; }
  bool operator!=(const Entity& other) const { return _id != other._id; }

 private:
  int _id;
};

/*
 * System class
 */

export class System {
 public:
  System() = default;
  ~System() = default;

  void addEntity(Entity entity);

  void removeEntityFromSystem(Entity entity);

  std::vector<Entity> getSystemEntities() const { return _entities; }
  const Signature& getComponentSignature() const { return _componentSignature; }

  template <typename T>
  void requireComponent() {
    const auto componentId = Component<T>::getId();
    _componentSignature.set(componentId);
  }

 private:
  Signature _componentSignature;
  std::vector<Entity> _entities;
};

void System::addEntity(Entity entity) { _entities.push_back(entity); }

void System::removeEntityFromSystem(Entity entity) {
  _entities.erase(
      std::remove_if(_entities.begin(), _entities.end(), [&entity](Entity& other) { return other == entity; }),
      _entities.end());
}

////////////////////////////////////////////////////////////////////////////////////////
// Registry
////////////////////////////////////////////////////////////////////////////////////////
// The registry class is responsible for creating entities and managing the component pools.
////////////////////////////////////////////////////////////////////////////////////////

export class Registry {
 public:
  Registry() = default;

  void update();
  Entity createEntity();

  template <typename T, typename... TArgs>
  void addComponent(Entity entity, TArgs&&... args);

  template <typename T>
  void removeComponent(Entity entity);

  template <typename T>
  bool hasComponent(Entity entity) const;

  //  void addComponent(Entity entity, IPool* pool);
  //  void getComponent(Entity entity);
  //  void addSystem(System* system);
  //  void addEntityToSystem(Entity entity);

 private:
  // Keeps track number of the entities
  int _numEntities = 0;

  // Vector of component pools, each pool stores a specific component type
  // Vector index is the component id
  // Pool index is the component id
  std::vector<IPool*> _componentPools;
  // Vector of signatures, each signature represents the components an entity has
  // Vector index is the entity id
  std::vector<Signature> _entityComponentSignatures;
  std::unordered_map<std::type_index, System*> _entityIdToIndex;

  std::set<Entity> _entitiesToBeAdded;
  std::set<Entity> _entitiesToBeKilled;
};

Entity Registry::createEntity() {
  int entityId = _numEntities++;

  if (entityId >= _entityComponentSignatures.size()) {
    _entityComponentSignatures.resize(entityId + 1);
  }

  Entity entity(entityId);
  _entitiesToBeAdded.insert(entity);

  Logger::log("Entity created with id: " + std::to_string(entityId));

  return entity;
}

void Registry::update() {
  // Actually add/remove  the entities between frames
  // TODO: Add entities to systems
}

template <typename T>
bool Registry::hasComponent(Entity entity) const {
    const auto componentId = Component<T>::getId();
    const auto entityId = entity.getId();

  return _entityComponentSignatures[entityId].test(componentId);
}

template <typename T>
void Registry::removeComponent(Entity entity) {
  const auto componentId = Component<T>::getId();
  const auto entityId = entity.getId();

  _entityComponentSignatures[entityId].set(componentId, false);
}

template <typename T, typename... TArgs>
void Registry::addComponent(Entity entity, TArgs&&... args) {
  const auto componentId = Component<T>::getId();
  const auto entityId = entity.getId();

  // if the component pool does not exist, create it
  if (componentId >= _componentPools.size()) {
    _componentPools.resize(componentId + 1, nullptr);
  }

  // Need to create a new component pool
  if (!_componentPools[componentId]) {
    Pool<T>* newComponentPool = new Pool<T>();
    _componentPools[componentId] = newComponentPool;
  }

  // Get the needed component pool
  Pool<T>* componentPool = Pool<T>(_componentPools[componentId]);

  if (entityId >= componentPool->size()) {
    componentPool->resize(_numEntities);
  }
  // Create component of type T and forward args to it's constructor
  T newComponent(std::forward<TArgs>(args)...);

  // Set component to the pool
  componentPool->set(entityId, newComponent);

  // Change the signature of the entity to include the component
  _entityComponentSignatures[entityId].set(componentId, true);
}
