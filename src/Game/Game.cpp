#include "Game.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include "src/Logger/Logger.h"
#include <stdlib.h>

Game::Game()
{
	
	Logger::Log("Engine Constructor Called!");
	
	isRunning = false;
}

Game::~Game()
{
	Logger::Log("Engine Destructor Called!");
}

void Game::Init() {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		Logger::Log("Error initializing SDL");
		return;
	}
	
	SDL_DisplayMode displayMode;

	// Get the current display mode and resolution of the first display.
	SDL_GetCurrentDisplayMode(0, &displayMode);

	windowWidth = 800;
	windowHeight = 600;
	
	window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);

	if (!window) {
		Logger::Error("Error creating SDL window");
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		Logger::Error("Error creating SDL renderer");
	}
	// Setting the video mode to "real" fullscreen
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	SDL_SetWindowFullscreen(window, SDL_WINDOW_SHOWN);

	// Set is running to true if you wre able to create the window and renderer.
	isRunning = true;
}

void Game::Run() {

	Setup();
	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

void Game::ProcessInput() {

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

glm::vec2 playerPos;
glm::vec2 playerVel;

void Game::Setup() {
	// Initialize game objects
	playerPos = glm::vec2(10.0, 20.0);
	playerVel = glm::vec2(10.0, 5.0);

}
void Game::Update()
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
	playerPos.x += playerVel.x * deltaTime;
	playerPos.y += playerVel.y * deltaTime;
	
}

void Game::Render() {
	// Set background color to red.
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);

	// Clear the back buffer.
	SDL_RenderClear(renderer);


	// Loaded the PNG from our Assets folder.
	SDL_Surface* surface = IMG_Load("assets/images/tank-tiger-right.png");

	// FIXME: Error message printing infinitely to the console.
	// Error message will display if the image never loaded.
	if (!surface) {
		Logger::Error("Error loading image. Please Check file path.");
		return;
	}

	// Create a texture from the surface.
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	// Free the surface since we are done with it.
	SDL_FreeSurface(surface);

	// Destination Recatangle where the texture will end up.
	SDL_Rect dstRectangle = { static_cast<int>(playerPos.x),static_cast<int>(playerPos.y),32,32 };


	// Draw the texture.
	SDL_RenderCopy(renderer, texture, NULL, &dstRectangle);
	SDL_DestroyTexture(texture);

	// Show the back buffer.
	SDL_RenderPresent(renderer);
}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}



