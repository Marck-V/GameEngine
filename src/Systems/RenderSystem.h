#pragma once

#include "../ECS/ECS.h"
#include "../Components/Components.h"
#include <SDL.h>
#include <SDL_image.h>
#include "../AssetManager/AssetManager.h"


class RenderSystem : public System {

public:
	RenderSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}


	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager) {
		// TODO: Sort all the entities of our system by the z-index.

		std::vector<Entity> sortedEntities = GetSystemEntities();

		// Sort by the Z-index.
		std::sort(sortedEntities.begin(), sortedEntities.end(), [](Entity& a, Entity& b) { return a.GetComponent<SpriteComponent>().zIndex < b.GetComponent<SpriteComponent>().zIndex; });
		
		// Loop through all entities that the system is interested in
		for (auto entity : sortedEntities) {
			
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto sprite = entity.GetComponent<SpriteComponent>();

			// Setting the source rectangle of our sprite.
			SDL_Rect srcRect = sprite.srcRect;

			// Set the destination rectangle of our sprite.
			SDL_Rect dstRect = { static_cast<int>(transform.position.x), static_cast<int>(transform.position.y), static_cast<int>(sprite.width * transform.scale.x), static_cast<int>(sprite.height * transform.scale.y) };

			// This version of SDL_RenderCopy allows us to flip the sprite and rotate it. We are using this since we have a rotation variable in our transform component.
			SDL_RenderCopyEx(renderer, assetManager->GetTexture(sprite.assetID), &srcRect, &dstRect, transform.rotation, NULL, SDL_FLIP_NONE);
		}
	}
};