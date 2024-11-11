module;

#include <bitset>
#include <memory>
#include <set>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <deque>

export module ecs;
import pool;
import logger;

export class Registry;

constexpr size_t MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct BaseComponent {
protected:
  static size_t _nextId;
};

size_t BaseComponent::_nextId = 0;

template <typename T>
class Component : public BaseComponent {
public:
  static size_t getId() {
    static size_t id = _nextId++;
    return id;
  }
};

export class Entity {
public:
  Entity(size_t id) : _id(id) {
  };

  Entity(const Entity& other) = default;

  Entity& operator=(const Entity& other) = default;

  size_t getId() const { return _id; }
  void kill();

  bool operator==(const Entity& other) const { return _id == other._id; }
  bool operator>(const Entity& other) const { return _id > other._id; }
  bool operator<(const Entity& other) const { return _id < other._id; }
  bool operator!=(const Entity& other) const { return _id != other._id; }


  template <typename TComponent, typename... TArgs>
  void addComponent(TArgs&&... args);
  template <typename TComponent, typename... TArgs>
  void removeComponent();
  template <typename TComponent, typename... TArgs>
  bool hasComponent() const;
  template <typename TComponent, typename... TArgs>
  TComponent& getComponent();

  class Registry* registry;

private:
  size_t _id;
};


/*
 * System class
 */

export class System {
private:
  Signature _componentSignature;
  std::vector<Entity> _entities;

public:
  System() {
    Logger::err(_entities.size() + "SIZE");
  };
  ~System() = default;

  void addEntity(Entity entity);

  void removeEntityFromSystem(Entity entity);

  std::vector<Entity> getSystemEntities() const {
    return _entities;
  }


  [[nodiscard]] const Signature& getComponentSignature() const { return _componentSignature; }

  template <typename T>
  void requireComponent() {
    const auto componentId = Component<T>::getId();
    _componentSignature.set(componentId);
  }
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


  //Move constructor
  Registry(Registry&& other) noexcept {
    _numEntities = other._numEntities;
    _componentPools = std::move(other._componentPools);
    _entityComponentSignatures = std::move(other._entityComponentSignatures);
    _systems = std::move(other._systems);
    _entitiesToBeAdded = std::move(other._entitiesToBeAdded);
    _entitiesToBeKilled = std::move(other._entitiesToBeKilled);
  }

  //copy constructor
  Registry(const Registry& other) {
    _numEntities = other._numEntities;
    _componentPools = other._componentPools;
    _entityComponentSignatures = other._entityComponentSignatures;
    _systems = other._systems;
    _entitiesToBeAdded = other._entitiesToBeAdded;
    _entitiesToBeKilled = other._entitiesToBeKilled;
  }

  void update();

  // Entity Management
  Entity createEntity();

  void killEntity(Entity entity);


  // Component Management
  template <typename T, typename... TArgs>
  void addComponent(Entity entity, TArgs&&... args);

  template <typename T>
  void removeComponent(Entity entity);

  template <typename T>
  [[nodiscard]] bool hasComponent(Entity entity) const;

  template <typename T>
  [[nodiscard]] T& getComponent(Entity entity) const;
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
  void removeEntityFromSystems(Entity entity);

private:
  // Keeps track number of the entities
  int _numEntities = 0;

  //Freed Ids that would be reused on create entity
  std::deque<size_t> _freedIds;

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

  // Entity tags (one tag name per entity)
  std::unordered_map<std::string, Entity> entityPerTag;
  std::unordered_map<int, std::string> tagPerEntity;

  // Entity groups (a set of entities pre group name)
  std::unordered_map<std::string, std::set<Entity>> entitiesPerGroup;
  std::unordered_map<int, std::string> groupPerEntity;
};

Entity Registry::createEntity() {
  size_t entityId;
  if (_freedIds.empty()) {
    entityId = _numEntities++;

    if (entityId >= _entityComponentSignatures.size()) {
      _entityComponentSignatures.resize(entityId + 1);
    }
  } else {
    entityId = _freedIds.front();
    _freedIds.pop_front();
  }

  Entity entity(entityId);
  entity.registry = this;
  _entitiesToBeAdded.insert(entity);
  if (entityId >= _entityComponentSignatures.size()) {
    _entityComponentSignatures.resize(entityId + 1);
  }

  Logger::log("Entity created with id: " + std::to_string(entityId));

  return entity;
}

void Registry::killEntity(Entity entity) {
  _entitiesToBeKilled.insert(entity);
}


void Registry::update() {
  // Actually add/remove  the entities between frames
  for (const auto& entity : _entitiesToBeAdded) {
    addEntityToSystem(entity);
    Logger::log("Entity added to system: " + std::to_string(entity.getId()));
  }
  _entitiesToBeAdded.clear();

  for (auto entity : _entitiesToBeKilled) {
    removeEntityFromSystems(entity);

    _entityComponentSignatures[entity.getId()].reset();
    // add to freed ids
    _freedIds.push_back(entity.getId());
  }
  _entitiesToBeKilled.clear();
}

void Registry::addEntityToSystem(Entity entity) {
  const auto entityId = entity.getId();

  const auto& entitySignature = _entityComponentSignatures[entityId];

  // loop all the systems
  for (auto& system : _systems) {
    const auto& systemSignature = system.second->getComponentSignature();

    // Check if the entity signature matches the system signature
    if ((entitySignature & systemSignature) == systemSignature) {
      system.second->addEntity(entity);
    }
  }
}

void Registry::removeEntityFromSystems(Entity entity) {
  for (auto& system : _systems) {
    system.second->removeEntityFromSystem(entity);
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
  Logger::log("Component removed from entity: " + std::to_string(entityId) + " of type" + typeid(T).name());
}

template <typename T>
T& Registry::getComponent(Entity entity) const {
  const auto componentId = Component<T>::getId();
  const auto entityId = entity.getId();
  auto componentPool = std::static_pointer_cast<Pool<T>>(_componentPools[componentId]);
  return componentPool->get(entityId);
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
  auto componentPool = std::static_pointer_cast<Pool<T>>(_componentPools[componentId]);

  if (entityId >= componentPool->getSize()) {
    componentPool->resize(_numEntities);
  }
  // Create component of type T and forward args to it's constructor
  T newComponent(std::forward<TArgs>(args)...);

  // Set component to the pool
  componentPool->set(entityId, newComponent);

  // Change the signature of the entity to include the component
  _entityComponentSignatures[entityId].set(componentId, true);
  Logger::log("Component added to entity: " + std::to_string(entityId) + " of type" + typeid(T).name());
}


template <typename TSystem, typename... TArgs>
void Registry::addSystem(TArgs&&... args) {
  auto system = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
  _systems.insert(std::make_pair(std::type_index(typeid(TSystem)), system));
}

template <typename TSystem>
void Registry::removeSystem() {
  auto system = _systems.find(std::type_index(typeid(TSystem)));
  _systems.erase(system);
}

template <typename TSystem>
bool Registry::hasSystem() const {
  return _systems.find(std::type_index(typeid(TSystem))) != _systems.end();
}

template <typename TSystem>
TSystem& Registry::getSystem() const {
  auto system = _systems.find(std::type_index(typeid(TSystem)));
  return *(std::static_pointer_cast<TSystem>(system->second));
}

/*
 * Entity class methods
 * TODO: Move to separate file or move up?
 */

void Entity::kill() {
  registry->killEntity(*this);
}

template <typename TComponent, typename... TArgs>
void Entity::addComponent(TArgs&&... args) {
  registry->addComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent, typename... TArgs>
void Entity::removeComponent() {
  registry->removeComponent<TComponent>(*this);
}

template <typename TComponent, typename... TArgs>
bool Entity::hasComponent() const {
  return registry->hasComponent<TComponent>(*this);
}

template <typename TComponent, typename... TArgs>
TComponent& Entity::getComponent() {
  return registry->getComponent<TComponent>(*this);
}

