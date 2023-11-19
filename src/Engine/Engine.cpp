#include "Engine.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include "../src/Components/Components.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include <stdlib.h>
#include <spdlog/spdlog.h>
#include <fstream>

Engine::Engine()
{	
	isRunning = false;
	manager = std::make_unique<Manager>(); // Creating a new instance of the manager class.
	assetManager = std::make_unique<AssetManager>(); // Creating a new instance of the asset container class.
	spdlog::info("Engine Constructor Called.");
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

	windowWidth = 1280;
	windowHeight = 720;
	
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

void Engine::LoadLevel(int level){

	// Add the systems that need to be processed in our game.
	manager->AddSystem<MovementSystem>();
	manager->AddSystem<RenderSystem>();

	// Adding the textures to the asset container.
	assetManager->AddTexture(renderer, "tank-image", "assets/images/tank-panther-right.png");
	assetManager->AddTexture(renderer, "truck-image", "assets/images/truck-ford-right.png");
	assetManager->AddTexture(renderer, "tilemap-image", "assets/tilemaps/jungle.png");

	// TODO: See if this can be improved by using a 2D array.
	 
	// Load the tilemap
	int tileSize = 32;
	double tileScale = 2.0;
	int mapNumColumns = 25;
	int mapNumRows = 20;

	std::fstream mapFile;
	mapFile.open("assets/tilemaps/jungle.map");
	
	// Loop through the map file and create the tiles.
	for (int row = 0; row < 20; row++) {
		for (int column = 0; column < 25; column++) {

			char ch;

			// Read the next character from the file.
			mapFile.get(ch);

			// Convert the char to an int and multiply it by the tile size.
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;

			// Ignore the comma and the space.
			mapFile.ignore();

			// Create an entity for each tile.
			Entity tile = manager->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(column * (tileSize * tileScale), row * (tileSize * tileScale)), glm::vec2(tileScale, tileScale), 0.0);
			tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, srcRectX, srcRectY);

		}
	}
	

	// Creating the entities.
	Entity tank = manager->CreateEntity();
	tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(50.0, 0));
	tank.AddComponent<SpriteComponent>("tank-image", 32, 32);

	// TODO: Add an error message that pops up if the file is not found.

	Entity truck = manager->CreateEntity();
	truck.AddComponent<TransformComponent>(glm::vec2(100.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
	truck.AddComponent<RigidBodyComponent>(glm::vec2(0, 50.0));
	truck.AddComponent<SpriteComponent>("truck-image", 32, 32);

}


void Engine::Setup() {
	LoadLevel(1);
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
	 
	// Invoke all the systems in the manager.
	manager->Update();

	// Updating game objects.
	manager->GetSystem<MovementSystem>().Update(deltaTime);

}

void Engine::Render() {
	// Set background color to red.
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);

	// Clear the back buffer.
	SDL_RenderClear(renderer);

	// Invoking the systems needed to render the game.
	manager->GetSystem<RenderSystem>().Update(renderer, assetManager);
	
	// Show the back buffer.
	SDL_RenderPresent(renderer);
}

void Engine::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}



