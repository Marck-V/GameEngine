#pragma once

#include "../ECS/ECS.h"
#include "../src/Components/Components.h"
#include <SDL.h>

class ProjectileSystem : public System {
public:
	ProjectileSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<ProjectileComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		
		eventBus->EmitEvent<KeyboardPressedEvent>(KeyboardPressedEvent(SDL_SCANCODE_SPACE));
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
				Entity projectile = manager->CreateEntity();
				projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
				projectile.AddComponent<RigidBodyComponent>(projectileComp.velocity);
				projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
				projectile.AddComponent<BoxColliderComponent>(4, 4);
				projectile.AddComponent<LifespanComponent>(projectileComp.projectileDuration);

				// Update the last shot time to the current milliseconds.
				projectileComp.lastShotTime = SDL_GetTicks();

			}

			
		}
	}

};