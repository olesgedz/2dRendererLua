module;

#include <bitset>
#include <memory>
#include <vector>

export module ecs;

import pool;

constexpr unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct BaseComponent {
protected:
  static int _nextId;
};

template <typename T>
class Component : public BaseComponent {
  static int getId() {
    static int id = _nextId++;
    return id;
  }
};

class Entity {
public:
  Entity(int id) : _id(id) {
  };

  Entity(const Entity& other) = default;

  Entity& operator=(const Entity& other) = default;

  int getId() const { return _id; }

  bool operator==(const Entity& other) const {
    return _id == other._id;
  }

  bool operator!=(const Entity& other) const {
    return _id != other._id;
  }

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

void System::addEntity(Entity entity) {
  _entities.push_back(entity);
}

void System::removeEntityFromSystem(Entity entity) {
  _entities.erase(std::remove_if(_entities.begin(), _entities.end(),
                                 [&entity](Entity& other) { return other == entity; }),
                  _entities.end());
}

////////////////////////////////////////////////////////////////////////////////////////
// Registry
////////////////////////////////////////////////////////////////////////////////////////
// The registry class is responsible for creating entities and managing the component pools.
////////////////////////////////////////////////////////////////////////////////////////


export class Registry {

public:
  Registry () = default;

  Entity createEntity();


private:

  // Keeps track number of the entities
  int numEntities = 0;

  // Vector of component pools, each pool stores a specific component type
  // Vector index is the component id
  // Pool index is the component id
  std::vector<IPool *> _componentPools;
  // Vector of signatures, each signature represents the components an entity has
  std::vector<Signature> _entityComponentSignatures;
};