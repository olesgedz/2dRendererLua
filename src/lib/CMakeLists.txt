cmake_minimum_required(VERSION 3.28)

set(Target EngineLib)

project(${Target})

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)

if (DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
endif ()

file(GLOB_RECURSE source_files "src/*.cpp")
file(GLOB_RECURSE header_files "src/*.hpp")
file(GLOB_RECURSE module_files "src/*.cppm")
file(GLOB_RECURSE module_interface_files "src/*.ixx")

# That is a hack to make sure that all files are in the same list, but target source does not include module files type yet
set(all_module_files ${module_files} ${module_interface_files})

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)


add_library(${Target} STATIC)

target_sources(${Target}
        PUBLIC
        ${source_files}     # Regular source file
        PUBLIC
        FILE_SET modules TYPE CXX_MODULES
        #        BASE_DIRS src
        FILES
        ${all_module_files}     # Module files
        PUBLIC
        FILE_SET headers TYPE HEADERS
        #        BASE_DIRS include
        FILES
        ${header_files}     # Header files
)

target_include_directories(${Target}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
)
target_link_libraries(${Target} PUBLIC glm::glm)

target_include_directories(${Target} PUBLIC ${LUA_INCLUDE_DIR})
target_include_directories(${Target} PUBLIC ${IMGUI_INCLUDE_DIRS})

target_link_libraries(${Target} PUBLIC ${LUA_LIBRARIES})

target_link_libraries(${Target} PUBLIC
        imgui::imgui
)

target_link_libraries(${Target} PUBLIC SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(${Target} PUBLIC $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
target_link_libraries(${Target} PUBLIC $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)
target_link_libraries(${Target} PUBLIC $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)
target_link_libraries(${Target} PUBLIC sol2)


### Tests
#enable_testing()
#
#file(GLOB_RECURSE test_source_files "test/src/*.cpp")
#file(GLOB_RECURSE test_header_files "test/src/*.h")
#find_package(GTest CONFIG REQUIRED)
#
#add_executable(app_test ${test_source_files} ${test_header_files})
#
#target_link_libraries(
#        app_test
#        PRIVATE
#        AppLib
#        GTest::gtest_main
#)
#
#target_include_directories(app_test
#        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/
#)
#
#include(GoogleTest)
#gtest_discover_tests(app_test)