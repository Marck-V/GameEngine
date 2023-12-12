#pragma once

#include "../EventBus/EventBus.h"
#include "../src/ECS/ECS.h"
#include "../src/Components/Components.h"
#include "../src/Events/Events.h"

class DamageSystem : public System {
public:
	DamageSystem() {
		RequireComponent<BoxColliderComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
	}

	void OnCollision(CollisionEvent& event) {
		spdlog::info("Damage system received a collision event detected between Entity ID: {} and Entity ID: {}", event.entityA.GetID(), event.entityB.GetID());
		Entity a = event.entityA;
		Entity b = event.entityB;


	}

	void Update() {

	}
};