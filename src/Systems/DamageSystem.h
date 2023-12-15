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

		if (a.IsInGroup("projectiles") && b.HasTag("player")) {
			OnProjectileHitsPlayer(a, b);
		}

		if (b.IsInGroup("projectiles") && a.HasTag("player")) {
			OnProjectileHitsPlayer(b, a);
		}

		if (a.IsInGroup("projectiles") && b.IsInGroup("enemies")) {

		}

		if (b.IsInGroup("projectiles") && a.IsInGroup("enemies")) {

		}
	}

	void OnProjectileHitsPlayer(Entity projectile, Entity player) {
		auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

		if (!projectileComponent.isFriendly) {
			auto& healthComponent = player.GetComponent<HealthComponent>();

			healthComponent.healthPercentage -= projectileComponent.hitPercentageDamage;

			if (healthComponent.healthPercentage <= 0) {
				player.Kill();
			}
		}
	}

	void Update() {

	}
};