#pragma once

#include "../src/ECS/ECS.h"
#include "../src/Components/Components.h"

class LifespanSystem : public System {
public:
	LifespanSystem() {
		RequireComponent<LifespanComponent>();
	}

	void Update() {
		for (auto entity : GetSystemEntities()) {
			auto lifespan = entity.GetComponent<LifespanComponent>();
			
			if(SDL_GetTicks() - lifespan.startTime > lifespan.lifespanDuration) {
				entity.Kill();
				spdlog::info("Projectile destroyed");
			}
		}
	}
};
