#pragma once

#include <SDL.h>
#include <../src/ECS/ECS.h>
#include <../src/AssetManager/AssetManager.h>
#include "../src/EventBus/EventBus.h"

const int FPS = 60;
const int FRAME_TARGET_TIME = 1000 / FPS;

class Engine
{
	private:
		
		bool isRunning;
		bool isDebugMode;
		int msPrevFrame = 0;
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Rect camera;
		
		std::unique_ptr<Manager> manager; // Unique pointer to the manager class.
		std::unique_ptr<AssetManager> assetManager; // Unique pointer to the asset container class.
		std::unique_ptr<EventBus> eventBus; // Unique pointer to the event bus class.

	public:
		Engine();
		~Engine();
		void Init();
		void Run();
		void ProcessInput();
		void Update();
		void Render();
		void Destroy();
		void Setup(); 

		static int windowWidth;
		static int windowHeight;
		static int mapWidth;
		static int mapHeight;

};

