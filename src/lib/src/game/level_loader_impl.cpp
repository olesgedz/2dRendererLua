module;

#include <SDL_image.h>
#include <imgui.h>

#include <fstream>
#include <glm/glm.hpp>
#include <sol/sol.hpp>

module level_loader;

import components;
import logger;
import settings;
import game;

LevelLoader::LevelLoader() {}

LevelLoader::~LevelLoader() {}

void LevelLoader::loadLevel(const Resources& resources, int levelNumber) {
  sol::load_result result = resources.lua->load_file((resources.assetsPath / "scripts/level").string() +
                                                     std::to_string(levelNumber) + ".lua");

  if (!result.valid()) {
    sol::error err = result;
    std::string errMsg = err.what();
    Logger::err("Failed to load level script: " + errMsg);
    return;
  }

  resources.lua->script_file((resources.assetsPath / "scripts/level").string() + std::to_string(levelNumber) + ".lua");
  sol::state& lua = *resources.lua;
  sol::table level = lua["Level"];

  // Reading the level asstes

  sol::table assets = level["assets"];

  int i = 0;
  while (true) {
    sol::optional<sol::table> asset = assets[i];
    if (!asset) {
      break;
    }
    std::string type;
    std::string id;
    std::string path;

    if (asset.value()["type"].valid())
      type = asset.value()["type"];
    else
      Logger::err("Asset type not found");

    if (asset.value()["id"].valid())
      id = asset.value()["id"];
    else
      Logger::err("Asset id not found");

    if (asset.value()["path"].valid())
      path = asset.value()["path"];
    else
      Logger::err("Asset path not found");

    if (type == "texture") {
      resources.assetStorage->addTexture(id, resources.assetsPath / path, resources.renderer);
    } else if (type == "font") {
      resources.assetStorage->addFont(id, resources.assetsPath / path, 16);
    }

    i++;
  }

  // Read TileMap

  sol::table tileMap = level["tilemap"];
  std::string mapFilePath = tileMap["path"];
  std::string mapTextureId = tileMap["texture_asset_id"];
  int mapNumRows = tileMap["num_rows"];
  int mapNumCols = tileMap["num_cols"];
  int tileSize = tileMap["tile_size"];
  double mapScale = tileMap["scale"];
  std::fstream mapFile;

  mapFile.open(resources.assetsPath / mapFilePath);
  if (!mapFile) {
    Logger::err("Failed to open map file");
    return;
  }

  for (int y = 0; y < mapNumRows; y++) {
    for (int x = 0; x < mapNumCols; x++) {
      char ch;
      mapFile.get(ch);
      int srcRectY = std::atoi(&ch) * tileSize;
      mapFile.get(ch);
      int srcRectX = std::atoi(&ch) * tileSize;
      mapFile.ignore();

      Entity tile = resources.registry->createEntity();
      tile.addComponent<TransformComponent>(glm::vec2(x * (mapScale * tileSize), y * (mapScale * tileSize)),
                                            glm::vec2(mapScale, mapScale), 0.0);
      tile.addComponent<SpriteComponent>(mapTextureId, 0, glm::vec2(tileSize, tileSize), false,
                                         glm::vec2(srcRectX, srcRectY));
      tile.group("tiles");
    }
  }

  mapFile.close();

  Game::mapHeight = mapNumRows * tileSize * mapScale;
  Game::mapWidth = mapNumCols * tileSize * mapScale;

  // Reading the entities
  sol::table entities = level["entities"];

  i = 0;
  while (true) {
    sol::optional<sol::table> hasEntity = entities[i];
    if (!hasEntity) {
      break;
    }

    sol::table entity = entities[i];

    Entity newEntity = resources.registry->createEntity();

    // Tag
    sol::optional<std::string> tag = entity["tag"];
    if (tag) {
      newEntity.tag(tag.value());
      Logger::log("Tag: " + tag.value() + " added: " + newEntity.getTag());
    }

    // Group
    sol::optional<std::string> group = entity["group"];
    if (group) {
      newEntity.group(group.value());
    }

    // Components
    sol::optional<sol::table> hasComponents = entity["components"];
    if (hasComponents != sol::nullopt) {
      // Transform
      sol::optional<sol::table> transform = entity["components"]["transform"];
      if (transform != sol::nullopt) {
        newEntity.addComponent<TransformComponent>(
            glm::vec2(entity["components"]["transform"]["position"]["x"],
                      entity["components"]["transform"]["position"]["y"]),
            glm::vec2(entity["components"]["transform"]["scale"]["x"].get_or(1.0),
                      entity["components"]["transform"]["scale"]["y"].get_or(1.0)),
            entity["components"]["transform"]["rotation"].get_or(0.0));
      }

      // RigidBody
      sol::optional<sol::table> rigidbody = entity["components"]["rigidbody"];
      if (rigidbody != sol::nullopt) {
        newEntity.addComponent<RigidBodyComponent>(
            glm::vec2(entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
                      entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)));
      }

      // Sprite
      sol::optional<sol::table> sprite = entity["components"]["sprite"];
      if (sprite != sol::nullopt) {
        newEntity.addComponent<SpriteComponent>(entity["components"]["sprite"]["texture_asset_id"],
                                                entity["components"]["sprite"]["z_index"].get_or(1),
                                                glm::vec2(entity["components"]["sprite"]["width"].get_or(0),
                                                          entity["components"]["sprite"]["height"].get_or(0)),
                                                // entity["components"]["sprite"]["color"].get_or(glm::vec4(255.f)),
                                                entity["components"]["sprite"]["fixed"].get_or(false),
                                                glm::vec2(entity["components"]["sprite"]["src_rect_x"].get_or(0),
                                                          entity["components"]["sprite"]["src_rect_y"].get_or(0)));
      }

      // Animation
      sol::optional<sol::table> animation = entity["components"]["animation"];
      if (animation != sol::nullopt) {
        newEntity.addComponent<AnimationComponent>(entity["components"]["animation"]["num_frames"].get_or(1),
                                                   entity["components"]["animation"]["speed_rate"].get_or(1));
      }

      // BoxCollider
      sol::optional<sol::table> collider = entity["components"]["boxcollider"];
      if (collider != sol::nullopt) {
        newEntity.addComponent<BoxColliderComponent>(
            glm::vec2(entity["components"]["boxcollider"]["width"], entity["components"]["boxcollider"]["height"]),
            glm::vec2(entity["components"]["boxcollider"]["offset"]["x"].get_or(0),
                      entity["components"]["boxcollider"]["offset"]["y"].get_or(0)));
      }

      // Health
      sol::optional<sol::table> health = entity["components"]["health"];
      if (health != sol::nullopt) {
        newEntity.addComponent<HealthComponent>(
            static_cast<int>(entity["components"]["health"]["health_percentage"].get_or(100)));
      }

      // ProjectileEmitter
      sol::optional<sol::table> projectileEmitter = entity["components"]["projectile_emitter"];
      if (projectileEmitter != sol::nullopt) {
        newEntity.addComponent<ProjectileEmitterComponent>(
            glm::vec2(entity["components"]["projectile_emitter"]["projectile_velocity"]["x"],
                      entity["components"]["projectile_emitter"]["projectile_velocity"]["y"]),
            static_cast<int>(entity["components"]["projectile_emitter"]["repeat_frequency"].get_or(1)) * 1000,
            static_cast<int>(entity["components"]["projectile_emitter"]["projectile_duration"].get_or(10)) * 1000,
            static_cast<int>(entity["components"]["projectile_emitter"]["hit_percentage_damage"].get_or(10)),
            entity["components"]["projectile_emitter"]["friendly"].get_or(false));
      }

      // CameraFollow
      sol::optional<sol::table> cameraFollow = entity["components"]["camera_follow"];
      if (cameraFollow != sol::nullopt) {
        newEntity.addComponent<CameraFollowComponent>();
      }

      // KeyboardControlled
      sol::optional<sol::table> keyboardControlled = entity["components"]["keyboard_controller"];
      if (keyboardControlled != sol::nullopt) {
        newEntity.addComponent<KeyboardControlledComponent>(
            glm::vec2(entity["components"]["keyboard_controller"]["up_velocity"]["x"],
                      entity["components"]["keyboard_controller"]["up_velocity"]["y"]),
            glm::vec2(entity["components"]["keyboard_controller"]["right_velocity"]["x"],
                      entity["components"]["keyboard_controller"]["right_velocity"]["y"]),
            glm::vec2(entity["components"]["keyboard_controller"]["down_velocity"]["x"],
                      entity["components"]["keyboard_controller"]["down_velocity"]["y"]),
            glm::vec2(entity["components"]["keyboard_controller"]["left_velocity"]["x"],
                      entity["components"]["keyboard_controller"]["left_velocity"]["y"]));
      }
    }
    i++;
  }
}