module;

#include <sol/sol.hpp>

export module systems:script_system;

import ecs;
import components;
import event_bus;
import events;
import logger;

// CPP function bound in Lua

int getEntityPosition(Entity entity) {
  return 0;
}

void setEntityPosition(Entity entity, float x, float y) {
  if (entity.hasComponent<TransformComponent>()) {
    auto& transform = entity.getComponent<TransformComponent>();
    transform.position.x = x;
    transform.position.y = y;
  } else {
    Logger::err("SetEntityPosition(): Entity does not have a TransformComponent");
  }
}


std::tuple<double, double> GetEntityPosition(Entity entity) {
  if (entity.hasComponent<TransformComponent>()) {
    const auto transform = entity.getComponent<TransformComponent>();
    return std::make_tuple(transform.position.x, transform.position.y);
  } else {
    Logger::err("Trying to get the position of an entity that has no transform component");
    return std::make_tuple(0.0, 0.0);
  }
}

std::tuple<double, double> GetEntityVelocity(Entity entity) {
  if (entity.hasComponent<RigidBodyComponent>()) {
    const auto rigidbody = entity.getComponent<RigidBodyComponent>();
    return std::make_tuple(rigidbody.velocity.x, rigidbody.velocity.y);
  } else {
    Logger::err("Trying to get the velocity of an entity that has no rigidbody component");
    return std::make_tuple(0.0, 0.0);
  }
}

void SetEntityVelocity(Entity entity, double x, double y) {
  if (entity.hasComponent<RigidBodyComponent>()) {
    auto& rigidbody = entity.getComponent<RigidBodyComponent>();
    rigidbody.velocity.x = x;
    rigidbody.velocity.y = y;
  } else {
    Logger::err("Trying to set the velocity of an entity that has no rigidbody component");
  }
}

void SetEntityRotation(Entity entity, double angle) {
  if (entity.hasComponent<TransformComponent>()) {
    auto& transform = entity.getComponent<TransformComponent>();
    transform.rotation = angle;
  } else {
    Logger::err("Trying to set the rotation of an entity that has no transform component");
  }
}

void SetEntityAnimationFrame(Entity entity, int frame) {
  if (entity.hasComponent<AnimationComponent>()) {
    auto& animation = entity.getComponent<AnimationComponent>();
    animation.currentFrame = frame;
  } else {
    Logger::err("Trying to set the animation frame of an entity that has no animation component");
  }
}

void SetProjectileVelocity(Entity entity, double x, double y) {
  if (entity.hasComponent<ProjectileEmitterComponent>()) {
    auto& projectileEmitter = entity.getComponent<ProjectileEmitterComponent>();
    projectileEmitter.projectileVelocity.x = x;
    projectileEmitter.projectileVelocity.y = y;
  } else {
    Logger::err("Trying to set the projectile velocity of an entity that has no projectile emitter component");
  }
}

export class ScriptSystem : public System {
public:
  ScriptSystem() {
    requireComponent<ScriptComponent>();
  }

  void createLuaBindings(sol::state& lua) {
    lua.new_usertype<Entity>(
        "entity",
        "get_id", &Entity::getId,
        "has_tag", &Entity::hasTag,
        "tag", &Entity::tag,
        "destroy", &Entity::kill,
        "belongs_to_group", &Entity::belongsToGroup
        );
    lua.set_function("set_position", &setEntityPosition);
    lua.set_function("get_position", &getEntityPosition);
    lua.set_function("get_velocity", &GetEntityVelocity);
    lua.set_function("set_velocity", &SetEntityVelocity);
    lua.set_function("set_rotation", &SetEntityRotation);
    lua.set_function("set_projectile_velocity", &SetProjectileVelocity);
    lua.set_function("set_animation_frame", &SetEntityAnimationFrame);
  }

  void update(float deltaTime, int elapsedTime) {
    for (auto& entity : getSystemEntities()) {
      const auto scriptComponent = entity.getComponent<ScriptComponent>();

      scriptComponent.func(entity, deltaTime, elapsedTime);
    }
  }
};