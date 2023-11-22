#pragma once

#include "../ECS/ECS.h"
#include "../Components/Components.h"


class CollisionSystem : public System {
	public:
		CollisionSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
	}

		void Update() {
			auto entities = GetSystemEntities();

			// Looping through both entities and checking if they are colliding.
			for (std::vector<Entity>::iterator i = entities.begin(); i != entities.end(); i++) {
				Entity entity1 = *i;
				auto e1Transform = entity1.GetComponent<TransformComponent>();
				auto e1Collider = entity1.GetComponent<BoxColliderComponent>();

				// Looping through the rest of the entities that still need to be checked.
				for (auto j = entities.begin(); j != entities.end(); j++) {
					Entity entity2 = *j;

					// If the entities are the same, skip the rest of the loop.
					if (entity1 == entity2) {
						continue;
					}

					auto e2Transform = entity2.GetComponent<TransformComponent>();
					auto e2Collider = entity2.GetComponent<BoxColliderComponent>();

					// Perform AABB collision detection.
					bool collisionDetected = CheckAABBCollision(e1Transform.position.x + e1Collider.offset.x, e1Transform.position.y + e1Collider.offset.y, e1Collider.width, e1Collider.height, e2Transform.position.x + e2Collider.offset.x, e2Transform.position.y + e2Collider.offset.y, e2Collider.width, e2Collider.height);
					
					if (collisionDetected) {
						// Write a log message showing what entity id collided with each other using spdlog.
						spdlog::info("Entity ID: {} collided with Entity ID: {}", entity1.GetID(), entity2.GetID());
					}

				}
			}
	}
		// AABB Collision Detection
		bool CheckAABBCollision(double aX, double aY, double aWidth, double aHeight, double bX, double bY, double bWidth, double bHeight) {
			return (aX < bX + bWidth && aX + aWidth > bX && aY < bY + bHeight && aY + aHeight > bY);
		}
};