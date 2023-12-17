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
		//spdlog::info("Damage system received a collision event detected between Entity ID: {} and Entity ID: {}", event.entityA.GetID(), event.entityB.GetID());
		Entity a = event.entityA;
		Entity b = event.entityB;
		
		if (a.IsInGroup("projectiles") && b.HasTag("player")) {
			OnProjectileHitsPlayer(a, b);
		}

		if (b.IsInGroup("projectiles") && a.HasTag("player")) {
			OnProjectileHitsPlayer(b, a);
		}

		if (a.IsInGroup("projectiles") && b.IsInGroup("enemies")) {
			OnProjectileHitsEnemy(a, b);
		}

		if (b.IsInGroup("projectiles") && a.IsInGroup("enemies")) {
			OnProjectileHitsEnemy(b, a);
		}
	}

	void OnProjectileHitsPlayer(Entity projectile, Entity player) {
		const auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

		if (!projectileComponent.isFriendly) {
			// Reduce the health of the player by the projectile hitPercentDamage
			auto& health = player.GetComponent<HealthComponent>();

			// Subtract the health of the player
			health.healthPercentage -= projectileComponent.hitPercentDamage;
			
			// Kills the player when health reaches zero
			if (health.healthPercentage <= 0) {
				player.Kill();
			}

			// Kill the projectile
			projectile.Kill();
		}
	}

	void OnProjectileHitsEnemy(Entity projectile, Entity enemy) {
		auto projectileComponent = projectile.GetComponent<ProjectileComponent>();
		
		if (projectileComponent.isFriendly) {
			auto& healthComponent = enemy.GetComponent<HealthComponent>();

			healthComponent.healthPercentage -= projectileComponent.hitPercentDamage;

			if (healthComponent.healthPercentage <= 0) {
				enemy.Kill();
			}
			projectile.Kill();
		}
	}
};