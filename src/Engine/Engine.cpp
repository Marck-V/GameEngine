#include "Engine.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
//#include "src/Logger/Logger.h"
#include <stdlib.h>
#include <spdlog/spdlog.h>


Engine::Engine()
{
	
	spdlog::info("Engine Constructor Called.");
	
	isRunning = false;

	manager = new Manager();
}

Engine::~Engine()
{
	spdlog::info("Engine Destructor Called.");
}

void Engine::Init() {

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		spdlog::error("Error initializing SDL.");
		return;
	}
	
	SDL_DisplayMode displayMode;

	// Get the current display mode and resolution of the first display.
	SDL_GetCurrentDisplayMode(0, &displayMode);

	windowWidth = 800;
	windowHeight = 600;
	
	window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);

	if (!window) {
		spdlog::error("Error creating SDL window.");
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		spdlog::error("Error creating SDL renderer.");
	}

	// Setting the video mode to "real" fullscreen
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	SDL_SetWindowFullscreen(window, SDL_WINDOW_SHOWN);

	// Set is running to true if you wre able to create the window and renderer.
	isRunning = true;
}

void Engine::Run() {

	Setup();
	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

void Engine::ProcessInput() {

	SDL_Event event;

	// Gets the address to the event.
	while (SDL_PollEvent(&event)) {

		switch (event.type) {

			case SDL_QUIT:
				isRunning = false;
				break;

			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false;
				}
				break;
		}
	}

}



void Engine::Setup() {
	// Creatre the entities

	Entity tank = manager->CreateEntity();
	Entity wall = manager->CreateEntity();

	

}
void Engine::Update()
{
	// Getting the time to wait by subtracting the current time from the previous frame time.
	int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - msPrevFrame);

	// If the time to wait is greater than 0 and less than or equal to the frame target time, delay the loop.
	if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
		// TODO: Change this to a more accurate time delay for when I upgrade the engine to contain physics.
		SDL_Delay(timeToWait);
	}

	// Delta time is the difference in ticks from last frame converted to seconds.
	double deltaTime = (SDL_GetTicks() - msPrevFrame) / 1000.0;
	

	// Store the current frame time.
	 msPrevFrame = SDL_GetTicks();

	// Updating game objects.

}

void Engine::Render() {
	// Set background color to red.
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);

	// Clear the back buffer.
	SDL_RenderClear(renderer);

	// Load the PNG from our Assets folder.
	SDL_Surface* surface = IMG_Load("assets/images/tank-tiger-right.png");

	// Error message will display if the image never loaded.
	if (!surface) {
		spdlog::error("Error loading image. Please check file path.");
		isRunning = false;
		return;
	}

	// Show the back buffer.
	SDL_RenderPresent(renderer);
}

void Engine::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}



