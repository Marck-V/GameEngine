#pragma once

//#include "../ECS/ECS.h"
//#include "./src/Components/Components.h"
#include "ECS/ECS.h"
#include "Components/Components.h"
#include <SDL.h>

class AnimationSystem : public System {
	public:
		AnimationSystem(){
			RequireComponent<SpriteComponent>();
			RequireComponent<AnimationComponent>();
		}

		void Update() {
			for (auto entity : GetSystemEntities()) {
				auto& animation = entity.GetComponent<AnimationComponent>();
				auto& sprite = entity.GetComponent<SpriteComponent>();

				// Getting the current frame of the animation.
				animation.currentFrame = (SDL_GetTicks() - animation.startTime) * animation.frameRateSpeed / 1000 % animation.numFrames;
				sprite.srcRect.x = animation.currentFrame * sprite.width;
			}
		}
};