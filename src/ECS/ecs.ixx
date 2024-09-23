module;

#include <bitset>
#include <vector>

export module ecs;


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

class Registry {
};