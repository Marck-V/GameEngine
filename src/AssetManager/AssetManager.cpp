#include "AssetManager.h"
#include <spdlog/spdlog.h>
#include <SDL_image.h>

AssetManager::AssetManager()
{
	spdlog::info("AssetManager Constructor Called.");
}

AssetManager::~AssetManager()
{
	spdlog::info("AssetManager Destructor Called.");
	ClearAssets();
}

void AssetManager::ClearAssets()
{
	for (auto texture : textures) {
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();
}

void AssetManager::AddTexture(SDL_Renderer* renderer,const std::string& assetID, const std::string& filePath) {
	SDL_Surface* surface = IMG_Load(filePath.c_str());

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	// Emplace lets us add the texture to the map by passing in the assetID and the texture.
	textures.emplace(assetID, texture);
}

SDL_Texture* AssetManager::GetTexture(const std::string& assetID) const
{	
	return textures.at(assetID);

	spdlog::info("New texture added to the AssetManager with id = " + assetID);
}

