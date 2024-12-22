#include <gtest/gtest.h>

#include <sol/sol.hpp>

TEST(Lua, SimpleTest) {
  sol::state lua;

  lua.open_libraries(sol::lib::base);

  lua.script_file("../assets/scripts/simple_test.lua");

  int testVariable = lua["test_variable"];
  EXPECT_EQ(testVariable, 42);
}