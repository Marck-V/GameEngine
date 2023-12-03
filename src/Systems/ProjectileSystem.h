#pragma once

#include "../ECS/ECS.h"
#include "../src/Components/Components.h"
#include "../src/Events/Events.h"
#include <SDL.h>

class ProjectileSystem : public System {
public:
	ProjectileSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<ProjectileComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<KeyboardPressedEvent>(this, &ProjectileSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyboardPressedEvent& event) {
		if (event.symbol == SDLK_SPACE) {
			spdlog::info("Key Pressed: Spacebar.");
			for (auto entity : GetSystemEntities()) {
				if (entity.HasComponent<CameraComponent>()) {
					const auto projectileComp = entity.GetComponent<ProjectileComponent>();
					const auto transform = entity.GetComponent<TransformComponent>();
					const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

					glm::vec2 projectilePosition = transform.position;
					if (entity.HasComponent<SpriteComponent>()) {
						auto sprite = entity.GetComponent<SpriteComponent>();
						projectilePosition.x += (sprite.width / 2 * transform.scale.x);
						projectilePosition.y += (sprite.height / 2 * transform.scale.y);
					}

					glm::vec2 projectileVelocity = projectileComp.velocity;
					int directionX = 0;
					int directionY = 0;
					if (rigidbody.velocity.x > 0) directionX = +1;
					if (rigidbody.velocity.x < 0) directionX = -1;
					if (rigidbody.velocity.y > 0) directionY = +1;
					if (rigidbody.velocity.y < 0) directionY = -1;
					projectileVelocity.x = projectileVelocity.x * directionX;
					projectileVelocity.y = projectileVelocity.y * directionY;

					Entity projectile = entity.manager->CreateEntity();
					projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
					projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
					projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
					projectile.AddComponent<BoxColliderComponent>(4, 4);
					projectile.AddComponent<LifespanComponent>(projectileComp.projectileDuration);
				}
			}
		}
	}

	void Update(std::unique_ptr<Manager>& manager) {
		for (auto entity : GetSystemEntities()) {
			auto& projectileComp = entity.GetComponent<ProjectileComponent>();
			auto transform = entity.GetComponent<TransformComponent>();

			// Check if its time to re-emit the projectile
			if (SDL_GetTicks() - projectileComp.lastShotTime > projectileComp.repeatFrequency) {

				glm::vec2 projectilePosition = entity.GetComponent<TransformComponent>().position;
				if (entity.HasComponent<SpriteComponent>()) {
					auto sprite = entity.GetComponent<SpriteComponent>();
					projectilePosition.x += (sprite.width / 2 * transform.scale.x);
					projectilePosition.y += (sprite.height / 2 * transform.scale.y);
				}

				// Update the last shot time to the current milliseconds.
				projectileComp.lastShotTime = SDL_GetTicks();
			}
		}
	}

};