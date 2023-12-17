#pragma once

#include "../ECS/ECS.h"
#include "../src/Components/Components.h"
#include "../src/Events/Events.h"
#include <SDL.h>

class ProjectileSystem : public System {
public:
	ProjectileSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<ProjectileEmitterComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<KeyboardPressedEvent>(this, &ProjectileSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyboardPressedEvent& event) {
		if (event.symbol == SDLK_SPACE) {
			spdlog::info("Key Pressed: Spacebar.");
			for (auto entity : GetSystemEntities()) {
				if (entity.HasComponent<CameraComponent>()) {
					const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
					const auto transform = entity.GetComponent<TransformComponent>();
					const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

					glm::vec2 projectilePosition = transform.position;
					if (entity.HasComponent<SpriteComponent>()) {
						auto sprite = entity.GetComponent<SpriteComponent>();
						projectilePosition.x += (sprite.width / 2 * transform.scale.x);
						projectilePosition.y += (sprite.height / 2 * transform.scale.y);
					}

					glm::vec2 projectileVelocity = projectileEmitter.velocity;
					int directionX = 0;
					int directionY = 0;
					if (rigidbody.velocity.x > 0) directionX = +1;
					if (rigidbody.velocity.x < 0) directionX = -1;
					if (rigidbody.velocity.y > 0) directionY = +1;
					if (rigidbody.velocity.y < 0) directionY = -1;
					projectileVelocity.x = projectileVelocity.x * directionX;
					projectileVelocity.y = projectileVelocity.y * directionY;

					Entity projectile = entity.manager->CreateEntity();
					projectile.Group("projectiles");
					projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
					projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
					projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
					projectile.AddComponent<BoxColliderComponent>(4, 4);
					projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
				}
			}
		}
	}

	void Update(std::unique_ptr<Manager>& manager) {
		for (auto entity : GetSystemEntities()) {
			auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
			const auto transform = entity.GetComponent<TransformComponent>();

			if (projectileEmitter.repeatFrequency == 0) {
				continue;
			}

			// Check if its time to re-emit the projectile
			if (SDL_GetTicks() - projectileEmitter.lastShotTime > projectileEmitter.repeatFrequency) {

				glm::vec2 projectilePosition = transform.position;
				if (entity.HasComponent<SpriteComponent>()) {
					const auto sprite = entity.GetComponent<SpriteComponent>();
					projectilePosition.x += (transform.scale.x * sprite.width / 2);
					projectilePosition.y += (transform.scale.y * sprite.height / 2);
				}

				Entity projectile = entity.manager->CreateEntity();
				projectile.Group("projectiles");
				projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
				projectile.AddComponent<RigidBodyComponent>(projectileEmitter.velocity);
				projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
				projectile.AddComponent<BoxColliderComponent>(4, 4);
				projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);

				// Update the last shot time to the current milliseconds.
				projectileEmitter.lastShotTime = SDL_GetTicks();
			}
		}
	}

};