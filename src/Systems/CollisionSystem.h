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


				}
			}
	}
};