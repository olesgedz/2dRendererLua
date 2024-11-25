module;

#include <algorithm>
#include <bitset>
#include <deque>
#include <format>
#include <memory>
#include <set>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

module ecs;

// System class methods
void System::addEntity(Entity entity) { _entities.push_back(entity); }

void System::removeEntityFromSystem(Entity entity) {
  std::erase_if(_entities, [&entity](const Entity& other) { return other == entity; });
}

// Registry class methods
Registry::Registry(Registry&& other) noexcept {
  _numEntities = other._numEntities;
  _componentPools = std::move(other._componentPools);
  _entityComponentSignatures = std::move(other._entityComponentSignatures);
  _systems = std::move(other._systems);
  _entitiesToBeAdded = std::move(other._entitiesToBeAdded);
  _entitiesToBeKilled = std::move(other._entitiesToBeKilled);
}

// copy constructor
Registry::Registry(const Registry& other) {
  _numEntities = other._numEntities;
  _componentPools = other._componentPools;
  _entityComponentSignatures = other._entityComponentSignatures;
  _systems = other._systems;
  _entitiesToBeAdded = other._entitiesToBeAdded;
  _entitiesToBeKilled = other._entitiesToBeKilled;
}

// Tag management
void Registry::tagEntity(Entity entity, const std::string& tag) {
  entityPerTag.emplace(tag, entity);
  tagPerEntity.emplace(entity.getId(), tag);
}

bool Registry::entityHasTag(Entity entity, const std::string& tag) const {
  if (tagPerEntity.find(entity.getId()) == tagPerEntity.end()) {
    return false;
  }

  if (entityPerTag.find(tag) == entityPerTag.end()) {
    return false;
  }
  return entityPerTag.find(tag)->second == entity;
}

void Registry::removeEntityTag(Entity entity) {
  auto taggedEntity = tagPerEntity.find(entity.getId());
  if (taggedEntity != tagPerEntity.end()) {
    auto tag = taggedEntity->second;
    entityPerTag.erase(tag);
    tagPerEntity.erase(taggedEntity);
  }
}

Entity Registry::getEntityByTag(const std::string& tag) const { return entityPerTag.at(tag); }

// Group management
void Registry::groupEntity(Entity entity, const std::string& group) {
  entitiesPerGroup.emplace(group, std::set<Entity>());
  entitiesPerGroup[group].emplace(entity);
  groupPerEntity.emplace(entity.getId(), group);
}

bool Registry::entityBelongsToGroup(Entity entity, const std::string& group) const {
  if (!entitiesPerGroup.contains(group)) {
    return false;
  }
  auto groupEntities = entitiesPerGroup.at(group);
  return groupEntities.find(entity.getId()) != groupEntities.end();
}

std::vector<Entity> Registry::getEntitiesByGroup(const std::string& group) const {
  auto& setOfEntities = entitiesPerGroup.find(group)->second;
  return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end());
}

void Registry::removeEntityFromGroup(Entity entity) {
  auto groupedEntity = groupPerEntity.find(entity.getId());
  if (groupedEntity != groupPerEntity.end()) {
    auto group = entitiesPerGroup.find(groupedEntity->second);
    if (group != entitiesPerGroup.end()) {
      group->second.erase(entity);
    }
    groupPerEntity.erase(groupedEntity);
  }
}

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

void Registry::killEntity(Entity entity) { _entitiesToBeKilled.insert(entity); }

void Registry::update() {
  // Actually add/remove  the entities between frames
  for (const auto& entity : _entitiesToBeAdded) {
    addEntityToSystem(entity);
    Logger::log("Entity added to system: " + std::to_string(entity.getId()));
  }
  if (!_entitiesToBeAdded.empty()) _entitiesToBeAdded.clear();

  for (auto entity : _entitiesToBeKilled) {
    removeEntityFromSystems(entity);
    Logger::log("Entity removed from system: " + std::to_string(entity.getId()));
    _entityComponentSignatures[entity.getId()].reset();

    // remove entites from pools
    for (auto& componentPool : _componentPools) {
      if (componentPool) {
        // TODO: check it
        componentPool->removeFromPull(entity.getId());
      }
    }
    // add to freed ids
    _freedIds.push_back(entity.getId());

    removeEntityTag(entity);
    removeEntityFromGroup(entity);
  }
  if (!_entitiesToBeKilled.empty()) _entitiesToBeKilled.clear();
}

void Registry::addEntityToSystem(Entity entity) const {
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

void Registry::removeEntityFromSystems(Entity entity) const {
  for (auto& system : _systems) {
    system.second->removeEntityFromSystem(entity);
  }
}

// Entity class methods
void Entity::tag(const std::string& tag) const { registry->tagEntity(*this, tag); }

bool Entity::hasTag(const std::string& tag) const { return registry->entityHasTag(*this, tag); }

void Entity::group(const std::string& group) const { registry->groupEntity(*this, group); }

bool Entity::belongsToGroup(const std::string& group) const { return registry->entityBelongsToGroup(*this, group); }

void Entity::kill() const { registry->killEntity(*this); }
