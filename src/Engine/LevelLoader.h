#pragma once
#include "src/ECS/ECS.h"
#include "src/AssetManager/AssetManager.h"
#include <SDL.h>
#include <memory>

class LevelLoader
{
	public:
	LevelLoader();
	~LevelLoader();
	void LoadLevel(const std::unique_ptr<Manager>& manager, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer,int level);
};