#pragma once

#include <SDL.h>

#include <../src/ECS/ECS.h>
#include <../src/AssetManager/AssetManager.h>

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

		std::unique_ptr<Manager> manager; // Unique pointer to the manager class.
		std::unique_ptr<AssetManager> assetManager; // Unique pointer to the asset container class.

	public:
		Engine();
		~Engine();
		void Init();
		void Run();
		void ProcessInput();
		void LoadLevel(int level);
		void Update();
		void Render();
		void Destroy();
		void Setup(); 

		int windowWidth;
		int windowHeight;
};

