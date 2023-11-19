#pragma once

#include "../src/ECS/ECS.h"
#include "../src/Components/Components.h"
#include <spdlog/spdlog.h>

class MovementSystem : public System {
	public:

		MovementSystem() {
			// These are the required components for the movement system.
			RequireComponent<TransformComponent>();
			RequireComponent<RigidBodyComponent>();
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

				//spdlog::info("Entity ID: {0}, Position: {1}, {2}", entity.GetID(), transform.position.x, transform.position.y);
			}
		}

};