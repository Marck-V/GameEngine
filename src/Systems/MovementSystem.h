#pragma once

#include "../EventBus/EventBus.h"
#include "../src/ECS/ECS.h"
#include "../src/Components/Components.h"
#include "../src/Events/Events.h"

class MovementSystem : public System {
	public:

		MovementSystem() {
			// These are the required components for the movement system.
			RequireComponent<TransformComponent>();
			RequireComponent<RigidBodyComponent>();
		}

		void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus) {
			eventBus->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::OnCollision);
		}
		
        void OnCollision(CollisionEvent& event) {
            Entity a = event.a;
            Entity b = event.b;
            

            if (a.IsInGroup("enemies") && b.IsInGroup("obstacles")) {
                OnEnemyHitsObstacle(a, b); // "a" is the enemy, "b" is the obstacle
            }

            if (a.IsInGroup("obstacles") && b.IsInGroup("enemies")) {
                OnEnemyHitsObstacle(b, a); // "b" is the enemy, "a" is the obstacle
            }
        }

        void OnEnemyHitsObstacle(Entity enemy, Entity obstacle) {
            if (enemy.HasComponent<RigidBodyComponent>() && enemy.HasComponent<SpriteComponent>()) {
                auto& rigidbody = enemy.GetComponent<RigidBodyComponent>();
                auto& sprite = enemy.GetComponent<SpriteComponent>();

                if (rigidbody.velocity.x != 0) {
                    rigidbody.velocity.x *= -1;
                    sprite.flip = (sprite.flip == SDL_FLIP_NONE) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
                }

                if (rigidbody.velocity.y != 0) {
                    rigidbody.velocity.y *= -1;
                    sprite.flip = (sprite.flip == SDL_FLIP_NONE) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
                }
            }
        }

		void Update(double deltaTime) {

			// Loop through all entities that the system is interested in
			for (auto entity : GetSystemEntities()) {
				// Get the components from the entity using the GetComponent function
				auto& transform = entity.GetComponent<TransformComponent>();
				const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

				// Update the position of the entity based on its velocity.
				transform.position.x += rigidbody.velocity.x * deltaTime;
				transform.position.y += rigidbody.velocity.y * deltaTime;

				bool isEntityOutsideMap = transform.position.x < 0 || transform.position.x > Engine::mapWidth || transform.position.y < 0 || transform.position.y > Engine::mapHeight;

				if (isEntityOutsideMap && !entity.HasTag("player")) {
					entity.Kill();
				}
			}
		}
};