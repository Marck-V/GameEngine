#pragma once

#include <map>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class AssetManager
{
	private:
		std::map<std::string, SDL_Texture*> textures;
		std::map<std::string, TTF_Font*> fonts;

	public:
		AssetManager();
		~AssetManager();

		void ClearAssets();

		void AddTexture(SDL_Renderer* renderer, const std::string& assetID, const std::string& filePath);
		SDL_Texture* GetTexture(const std::string& assetID) const;

		void AddFont(const std::string& assetID, const std::string& filePath, int fontSize);
		TTF_Font* GetFont(const std::string& assetID) const;
};