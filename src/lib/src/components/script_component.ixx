module;

#include <sol/sol.hpp>

export module script_component;

export struct ScriptComponent {
  sol::function func;

  ScriptComponent(sol::function func = sol::lua_nil) : func(func) {
  }
};