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

export class Entity {
public:
  Entity(int id) : _id(id) {
  };

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
  std::erase_if(_entities, [&entity](const Entity& other) { return other == entity; });
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

  // Entity Management
  Entity createEntity();


  // Component Management
  template <typename T, typename... TArgs>
  void addComponent(Entity entity, TArgs&&... args);

  template <typename T>
  void removeComponent(Entity entity);

  template <typename T>
  [[nodiscard]] bool hasComponent(Entity entity) const;


  // System Management
  template <typename TSystem, typename... TArgs>
  void addSystem(TArgs&&... args);

  template <typename TSystem>
  void removeSystem();

  template <typename TSystem>
  [[nodiscard]] bool hasSystem() const;

  template <typename TSystem>
  TSystem& getSystem() const;

  // Checks the component signature of the entity and adds it to the system if it matches
  void addEntityToSystem(Entity entity);

private:
  // Keeps track number of the entities
  int _numEntities = 0;

  // Vector of component pools, each pool stores a specific component type
  // Vector index is the component id
  // Pool index is the component id
  std::vector<std::shared_ptr<IPool>> _componentPools;
  // Vector of signatures, each signature represents the components an entity has
  // Vector index is the entity id
  std::vector<Signature> _entityComponentSignatures;

  std::unordered_map<std::type_index, std::shared_ptr<System>> _systems;

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
  if (entityId >= _entityComponentSignatures.size()) {
    _entityComponentSignatures.resize(entityId + 1);
  }

  Logger::log("Entity created with id: " + std::to_string(entityId));

  return entity;
}

void Registry::update() {
  // Actually add/remove  the entities between frames
  for (const auto& entity : _entitiesToBeAdded) {
    addEntityToSystem(entity);
    Logger::log("Entity added to system" + std::to_string(entity.getId()));
  }
  _entitiesToBeAdded.clear();
  // TODO: Remove entities to systems
}

void Registry::addEntityToSystem(Entity entity) {
  const auto entityId = entity.getId();

  const auto entitySignature = _entityComponentSignatures[entityId];

  // loop all the systems
  for (auto& system : _systems) {
    const auto systemSignature = system.second->getComponentSignature();

    // Check if the entity signature matches the system signature
    if ((entitySignature & systemSignature) == systemSignature) {
      system.second->addEntity(entity);
    }
  }
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
    std::shared_ptr<Pool<T>> newComponentPool = std::make_shared<Pool<T>>();
    _componentPools[componentId] = newComponentPool;
  }

  // Get the needed component pool
auto componentPool = Pool<T>(_componentPools[componentId]);

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


template <typename TSystem, typename... TArgs>
void Registry::addSystem(TArgs&&... args) {
  auto system = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
  _systems.insert(std::make_pair(std::type_index(typeid(TSystem)), system));
}

template <typename TSystem>
void Registry::removeSystem() {
  _systems.erase(std::type_index(typeid(TSystem)));
}

template <typename TSystem>
bool Registry::hasSystem() const {
  return _systems.contains(std::type_index(typeid(TSystem)));
}

template <typename TSystem>
TSystem& Registry::getSystem() const {
  return std::static_pointer_cast<TSystem>(_systems.at(std::type_index(typeid(TSystem))));
}