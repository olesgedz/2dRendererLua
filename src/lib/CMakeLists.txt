cmake_minimum_required(VERSION 3.28)

set(Target EngineLib)

project(${Target})

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)

if (DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
endif ()

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)


add_library(${Target} STATIC)

# Collect files
file(GLOB_RECURSE source_files "src/*.cpp")
file(GLOB_RECURSE header_files "src/*.hpp")
file(GLOB_RECURSE module_interface_files "src/*.ixx")

# Specify sources for the target
target_sources(${Target}
        PUBLIC
        FILE_SET modules TYPE CXX_MODULES
        FILES
        ${module_interface_files}
        PUBLIC
        FILE_SET headers TYPE HEADERS
        FILES
        ${header_files}
)

target_sources(${Target}
        PRIVATE ${source_files}        # Source files
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
target_link_libraries(${Target} PUBLIC SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(${Target} PUBLIC $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
target_link_libraries(${Target} PUBLIC $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)
target_link_libraries(${Target} PUBLIC $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)
target_link_libraries(${Target} PUBLIC sol2)
target_link_libraries(${Target} PRIVATE imgui::imgui)