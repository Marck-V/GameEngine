#pragma once

//#include "src/ECS/ECS.h"
//#include "src/AssetManager/AssetManager.h"
#include "ECS/ECS.h"
#include "AssetManager/AssetManager.h"
#include <SDL.h>
#include <memory>
#include <sol/sol.hpp>

class LevelLoader
{
	public:
	LevelLoader();
	~LevelLoader();
	void LoadLevel(sol::state& lua, const std::unique_ptr<Manager>& manager, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer,int levelNumber);
};