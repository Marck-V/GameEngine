#pragma once

#include <map>
#include <string>
#include <SDL.h>


class AssetManager
{
	private:
		std::map<std::string, SDL_Texture*> textures;

		//TODO: Creating a map for fonts and audio files.

	public:
		AssetManager();
		~AssetManager();

		void ClearAssets();
		void AddTexture(SDL_Renderer* renderer, const std::string& assetID, const std::string& filePath);
		SDL_Texture* GetTexture(const std::string& assetID) const;
};

