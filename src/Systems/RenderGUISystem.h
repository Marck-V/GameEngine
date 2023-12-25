#pragma once

#include "../ECS/ECS.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>

class RenderGUISystem : public System{
public:
	RenderGUISystem() = default;

	void Update(const std::unique_ptr<Manager>& manager, const SDL_Rect& camera) {
		ImGui::NewFrame();

		if (ImGui::Begin("Spawn Enemies")) {
			static int posX = 0;
			static int posY = 0;
			static int scaleX = 1;
			static int scaleY = 1;
			static float enemyRotation = 0;
			static int velX = 0;
			static int velY = 0;
			static int enemyHealth = 100;
			static float projAngle = 0;
			static float projSpeed = 0;
			static int projDamage = 10;
			static int enemyFireRate = 1;
			static int projDuration = 1;
			const char* items[] = { "tank-image", "truck-image" };
			static int item_current = 0;
			int msToSec = 1000;

			if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Combo("enemy sprite", &item_current, items, IM_ARRAYSIZE(items));
			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::InputInt("position x", &posX);
				ImGui::InputInt("position y", &posY);
				ImGui::SliderInt("scale x", &scaleX, 1, 5);
				ImGui::SliderInt("scale y", &scaleY, 1, 5);
				ImGui::SliderAngle("rotation (deg)", &enemyRotation, 0 , 360);
			}
			ImGui::Spacing();

			if(ImGui::CollapsingHeader("Rigid Body", ImGuiTreeNodeFlags_DefaultOpen)){
				ImGui::SliderInt("velocity x", &velX, 0, 100);
				ImGui::SliderInt("velocity y", &velY, 0, 100);
			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::SliderInt("Health %", &enemyHealth, 1, 100);
			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Projectile", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::SliderAngle("angle (deg)", &projAngle, 0, 360);
				ImGui::SliderFloat("speed (px/sec)", &projSpeed, 10, 500);
				ImGui::InputInt("fire rate (sec)", &enemyFireRate);
				ImGui::InputInt("duration (sec)", &projDuration);
				ImGui::InputInt("damage", &projDamage);
			}
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			
			if (ImGui::Button("Spawn Enemy")) {
				Entity enemy = manager->CreateEntity();
				enemy.Group("enemies");
				enemy.AddComponent<TransformComponent>(glm::vec2(posX, posY), glm::vec2(scaleX, scaleY), glm::degrees(enemyRotation));
				enemy.AddComponent<RigidBodyComponent>(glm::vec2(velX, velY));
				enemy.AddComponent<SpriteComponent>(items[item_current], 32, 32, 1);
				enemy.AddComponent<BoxColliderComponent>(32, 32);
				double projVelX = cos(projAngle) * projSpeed;
				double projVelY = sin(projAngle) * projSpeed;
				enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(projVelX, projVelY), enemyFireRate * msToSec, projDuration * msToSec, projDamage, false);
				enemy.AddComponent<HealthComponent>(enemyHealth);

			}
			
			ImGui::SameLine();
			if (ImGui::Button("Reset")) {

				posX = 0;
				posY = 0;
				scaleX = 1;
				scaleY = 1;
				enemyRotation = 0;
				velX = 0;
				velY = 0;
				enemyHealth = 100;
				projAngle = 0;
				projSpeed = 0;
				projDamage = 10;
				enemyFireRate = 1;
				projDuration = 1;
				item_current = 0;
			}
		}

		ImGui::End();

		// Display small window to keep track of the maps coordiantes using the mouse.
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
		ImGui::SetNextWindowBgAlpha(0.9f);
		if (ImGui::Begin("Map Coordinates", NULL, windowFlags)) {
			ImGui::Text("Mouse Position: (%.1f, %.1f)", ImGui::GetIO().MousePos.x + camera.x, ImGui::GetIO().MousePos.y + camera.y);
		}
		ImGui::End();

		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());
	}
};