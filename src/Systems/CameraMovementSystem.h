#pragma once

#include "../src/ECS/ECS.h"
#include "../src/Components/Components.h"

class CameraMovementSystem : public System {
public:
	CameraMovementSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<CameraComponent>();
	}

	void Update(SDL_Rect& camera) {
		for (auto entity : GetSystemEntities()) {
			auto& transform = entity.GetComponent<TransformComponent>();
			
			if (transform.position.x + (camera.w / 2) < Engine::mapWidth){
				camera.x = transform.position.x - (Engine::windowWidth / 2);
			}
			if (transform.position.y + (camera.h / 2) < Engine::mapHeight) {
				camera.y = transform.position.y - (Engine::windowHeight / 2);
			}
			
			// Keep the camera within the bounds of the map.
			camera.x = camera.x < 0 ? 0 : camera.x;
			camera.y = camera.y < 0 ? 0 : camera.y;
			camera.x = camera.x > camera.w ? camera.w : camera.x;
			camera.y = camera.y > camera.h ? camera.h : camera.y;
		}
	}

};