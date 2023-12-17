#pragma once

#include "../src/ECS/ECS.h"
#include "../src/Components/Components.h"

class LifespanSystem : public System {
public:
	LifespanSystem() {
		RequireComponent<ProjectileComponent>();
	}

	void Update() {
		for (auto entity : GetSystemEntities()) {
			auto projectile = entity.GetComponent<ProjectileComponent>();
			
			if(SDL_GetTicks() - projectile.startTime > projectile.duration) {
				entity.Kill();
			}
		}
	}
};
