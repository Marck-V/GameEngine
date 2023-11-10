#ifndef GAME_H
#define GAME_H

#include <SDL.h>

const int FPS = 60;
const int FRAME_TARGET_TIME = 1000 / FPS;

class Game
{
	private:
		
		bool isRunning;
		int msPrevFrame = 0;
		SDL_Window* window;
		SDL_Renderer* renderer;


	public:
		Game();
		~Game();
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

#endif