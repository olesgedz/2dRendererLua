
module;
#include <SDL2/SDL.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
export module systems:render_gui_system;

import components;
import logger;
import ecs;
import asset_storage;

export class RenderGUISystem : public System {
public:
  RenderGUISystem() = default;
  void update(SDL_Renderer* renderer, ImGuiIO& io) const;
};


void RenderGUISystem::update(SDL_Renderer* renderer, ImGuiIO& io) const {
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  if (ImGui::Begin("Spawn Enemies")) {
    ImGui::Text("Here we spawn new enemies");
  }

  ImGui::End();

  ImGui::Render();
  SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}