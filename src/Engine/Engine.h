#pragma once

#include <SDL.h>

const int FPS = 60;
const int FRAME_TARGET_TIME = 1000 / FPS;

class Engine
{
	private:
		
		bool isRunning;
		int msPrevFrame = 0;
		SDL_Window* window;
		SDL_Renderer* renderer;


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

		int windowWidth;
		int windowHeight;
};
