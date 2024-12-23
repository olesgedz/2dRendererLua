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

int cppFunction() {
  std::cout << "Hello from C++!" << std::endl;
  return 42;
}

TEST(Lua, CPPFunction) {
  sol::state lua;

  lua["cppFunction"] = cppFunction;
  lua.open_libraries(sol::lib::base);
  lua.script_file("../assets/scripts/simple_test.lua");
}