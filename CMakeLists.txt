cmake_minimum_required(VERSION 3.28)

set(Target ENGINE)

project(${Target})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES TRUE)

if (NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif ()

set(CMAKE_BUILD_TYPE Debug)

if (DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
endif ()

add_subdirectory(src/lib)

add_executable(${Target} src/app/src/main.cpp)

target_link_libraries(${Target} PRIVATE EngineLib)

## Tests
enable_testing()

file(GLOB_RECURSE test_source_files "test/src/*.cpp")
file(GLOB_RECURSE test_header_files "test/src/*.h")
find_package(GTest CONFIG REQUIRED)

add_executable(unit_tests ${test_source_files} ${test_header_files}
        test/src/damage_system_tests.cpp)

target_link_libraries(
        unit_tests
        PRIVATE
        EngineLib
        GTest::gtest_main
)

target_include_directories(unit_tests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

include(GoogleTest)
gtest_discover_tests(unit_tests)