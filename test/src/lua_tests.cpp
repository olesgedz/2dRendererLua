#include <gtest/gtest.h>

#include <glm/gtc/constants.hpp>
#include <sol/sol.hpp>

TEST(Lua, SimpleTest) {
  sol::state lua;

  lua.open_libraries(sol::lib::base);

  lua.script_file("../assets/scripts/simple_test.lua");

  int testVariable = lua["test_variable"];
  EXPECT_EQ(testVariable, 42);

  sol::function functionFactorial = lua["factorial"];
  int result = functionFactorial(5);
  EXPECT_EQ(result, 120);
}

int cppFunction() { return 42; }

TEST(Lua, CPPFunction) {
  sol::state lua;
  lua.open_libraries(sol::lib::base);

  lua["cpp_function"] = cppFunction;
  lua.script_file("../assets/scripts/simple_test.lua");

  int result = lua["cpp_function"]();
  EXPECT_EQ(result, 42);
}

void setData(const std::shared_ptr<sol::state>& lua) { (*lua)["cppVar"] = -42; }

TEST(Lua, Reference) {
  std::shared_ptr<sol::state> lua = std::make_shared<sol::state>();
  lua->open_libraries(sol::lib::base);
  setData(lua);
  lua->script_file("../assets/scripts/reference_test.lua");
  EXPECT_EQ((*lua)["cppVar"], -42);
}