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
		
		Entity a = event.a;
		Entity b = event.b;
		
        if (a.IsInGroup("projectiles") && b.HasTag("player")) {
            OnProjectileHitsPlayer(a, b); // "a" is the projectile, "b" is the player
            spdlog::info("Damage system received a collision event detected between Entity ID: {} and Entity ID: {}", event.a.GetID(), event.b.GetID());
        }

        if (b.IsInGroup("projectiles") && a.HasTag("player")) {
            OnProjectileHitsPlayer(b, a); // "b" is the projectile, "a" is the player
            spdlog::info("Damage system received a collision event detected between Entity ID: {} and Entity ID: {}", event.a.GetID(), event.b.GetID());
        }

        if (a.IsInGroup("projectiles") && b.IsInGroup("enemies")) {
            OnProjectileHitsEnemy(a, b); // "a" is the projectile, "b" is the enemy
        }

        if (b.IsInGroup("projectiles") && a.IsInGroup("enemies")) {
            OnProjectileHitsEnemy(b, a); // "b" is the projectile, "a" is the enemy
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
        const auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

        if (projectileComponent.isFriendly) {
            // Reduce the health of the enemy by the projectile hitPercentDamage
            auto& health = enemy.GetComponent<HealthComponent>();

            // Subtract the health of the enemy
            health.healthPercentage -= projectileComponent.hitPercentDamage;

            // Kills the enemy when health reaches zero
            if (health.healthPercentage <= 0) {
                enemy.Kill();
            }

            // Kill the projectile
            projectile.Kill();
        }
    }
};