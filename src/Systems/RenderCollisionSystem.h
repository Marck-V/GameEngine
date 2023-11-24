#pragma once

#include "../src/ECS/ECS.h"
#include "../src/Components/Components.h"
#include <SDL.h>

class RenderCollisionSystem : public System {
public:
	RenderCollisionSystem() {
		RequireComponent<BoxColliderComponent>();
		RequireComponent<TransformComponent>();
	}

void Update(SDL_Renderer* renderer) {
		for (auto entity : GetSystemEntities()){
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& collider = entity.GetComponent<BoxColliderComponent>();

			SDL_Rect rect = {
				(int)transform.position.x,
				(int)transform.position.y,
				collider.width,
				collider.height
			};

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &rect);
		
		}
	}
};