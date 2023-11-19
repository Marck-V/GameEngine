#pragma once

#include "../ECS/ECS.h"
#include "../Components/Components.h"
#include <SDL.h>
#include <SDL_image.h>
#include "../AssetManager/AssetManager.h"
#include <fstream>


class RenderSystem : public System {

public:
	RenderSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}


	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetStore) {
		for (auto entity : GetSystemEntities()) {
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto sprite = entity.GetComponent<SpriteComponent>();

			// Setting the source rectangle of our sprite.
			SDL_Rect srcRect = sprite.srcRect;

			// Set the destination rectangle of our sprite.
			SDL_Rect dstRect = { static_cast<int>(transform.position.x), static_cast<int>(transform.position.y), static_cast<int>(sprite.width * transform.scale.x), static_cast<int>(sprite.height * transform.scale.y) };

			// This version of SDL_RenderCopy allows us to flip the sprite and rotate it. We are using this since we have a rotation variable in our transform component.
			SDL_RenderCopyEx(renderer, assetStore->GetTexture(sprite.assetID), &srcRect, &dstRect, transform.rotation, NULL, SDL_FLIP_NONE);
		}
	}

	// jungle.map is 20 x 25 tiles
	
};