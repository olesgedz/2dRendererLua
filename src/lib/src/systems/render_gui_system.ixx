
module;
#include <SDL2/SDL.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <glm/glm.hpp>
#include <memory>

export module systems:render_gui_system;

import asset_storage;
import components;
import logger;
import ecs;
import asset_storage;

export class RenderGUISystem : public System {
public:
  RenderGUISystem() = default;
  void update(SDL_Renderer* renderer, ImGuiIO& io, const std::unique_ptr<Registry>& registry) const;

private:
  std::shared_ptr<AssetStorage> _assetStorage;
};


void RenderGUISystem::update(SDL_Renderer* renderer, ImGuiIO& io, const std::unique_ptr<Registry>& registry) const {
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  if (ImGui::Begin("Spawn Enemies")) {
    static int enemyXPos = 0;
    static int enemyYPos = 0;
    static int scaleX = 1;
    static int scaleY = 1;
    static int velX = 0;
    static int velY = 0;
    static int health = 100;
    static float rotation = 0.f;
    static float projAngle = 0.f;
    static float projSpeed = 100;
    static int projRepeat = 10;
    static int projDuration = 10;
    static int selectedSpriteIndex = 0;
    const char* sprites[] = {"tank-image", "truck-image"};

    if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::Combo("Texture id", &selectedSpriteIndex, sprites,
                   IM_ARRAYSIZE(sprites));
    }

    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::InputInt("position x", &enemyXPos);
      ImGui::InputInt("position y", &enemyYPos);
      ImGui::SliderInt("scale x", &scaleX, 1, 10);
      ImGui::SliderInt("scale y", &scaleY, 1, 10);
      ImGui::SliderAngle("rotation (deg)", &rotation, 0, 360);
    }

    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Rigid body", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::InputInt("velocity x", &velX);
      ImGui::InputInt("velocity y", &velY);
    }

    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Projectile emitter", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::SliderAngle("angle (deg)", &projAngle, 0, 360);
      ImGui::SliderFloat("Speed (px/sec)", &projSpeed, 0, 500);
      ImGui::InputInt("repeat (sec) x", &projRepeat);
      ImGui::InputInt("duration (sec)", &projDuration);
    }

    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::SliderInt("%", &health, 0, 100);
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Spawn Enemy")) {
      Entity enemy = registry->createEntity();

      enemy.group("enemies");
      enemy.addComponent<TransformComponent>(glm::vec2(enemyXPos, enemyYPos), glm::vec2(scaleX, scaleY),
                                             glm::degrees(rotation));
      enemy.addComponent<RigidBodyComponent>(glm::vec2(velX, velY));
      enemy.addComponent<SpriteComponent>(sprites[selectedSpriteIndex], 1, glm::vec2(32.f, 32.f));
      enemy.addComponent<BoxColliderComponent>(glm::vec2(32.f, 32.f));

      double projVecX = glm::cos(projAngle) * projSpeed;
      double projVecY = glm::sin(projAngle) * projSpeed;

      enemy.addComponent<ProjectileEmitterComponent>(glm::vec2(projVecX, projVecY), projRepeat * 1000,
                                                     projDuration * 1000, 10, false);
      enemy.addComponent<HealthComponent>(health);

      enemyXPos = 0;
      enemyYPos = 0;
      scaleX = 1;
      scaleY = 1;
      rotation = 0.f;
      projAngle = 0.f;
      projRepeat = 10;
      projDuration = 10;
      projSpeed = 100;
      health = 100;
    }
  }

  ImGui::End();

  ImGui::Render();
  SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}