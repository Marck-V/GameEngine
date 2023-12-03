#include "Engine.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include "../src/Components/Components.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include <stdlib.h>
#include <spdlog/spdlog.h>
#include <fstream>
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderCollisionSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileSystem.h"
#include "../Systems/LifespanSystem.h"
#include "../src/Events/Events.h"
#include "../src/EventBus/EventBus.h"

int Engine::windowWidth = 0;
int Engine::windowHeight = 0;
int Engine::mapWidth = 0;
int Engine::mapHeight = 0;

Engine::Engine()
{	
	isRunning = false;
	isDebugMode = false;
	manager = std::make_unique<Manager>(); // Creating a new instance of the manager class.
	assetManager = std::make_unique<AssetManager>(); // Creating a new instance of the asset container class.
	eventBus = std::make_unique<EventBus>(); // Creating a new instance of the event bus class.
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

	// Initialize the camera view with the entire window.
	camera.x = 0;
	camera.y = 0;
	camera.w = windowWidth;
	camera.h = windowHeight;


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
				if (event.key.keysym.sym == SDLK_w) {
					isDebugMode = !isDebugMode;
				}
				eventBus->EmitEvent<KeyboardPressedEvent>(event.key.keysym.sym);
				break;
		}
	}

}

void Engine::LoadLevel(int level){

	// Add the systems that need to be processed in our game.
	manager->AddSystem<MovementSystem>();
	manager->AddSystem<RenderSystem>();
	manager->AddSystem<AnimationSystem>();
	manager->AddSystem<CollisionSystem>();
	manager->AddSystem<RenderCollisionSystem>();
	manager->AddSystem<DamageSystem>();
	manager->AddSystem<KeyboardControlSystem>();
	manager->AddSystem<CameraMovementSystem>();
	manager->AddSystem<ProjectileSystem>();
	manager->AddSystem<LifespanSystem>();

	// Adding the textures to the asset container.
	assetManager->AddTexture(renderer, "tank-image", "assets/images/tank-panther-right.png");
	assetManager->AddTexture(renderer, "truck-image", "assets/images/truck-ford-right.png");
	assetManager->AddTexture(renderer, "tilemap-image", "assets/tilemaps/jungle.png");
	assetManager->AddTexture(renderer, "chopper-image", "assets/images/chopper-spritesheet.png");
	assetManager->AddTexture(renderer, "radar-image", "assets/images/radar.png");
	assetManager->AddTexture(renderer, "bullet-image", "assets/images/bullet.png");
	// TODO: See if this can be improved by using a 2D array.
	 
	// Load the tilemap
	int tileSize = 32;
	double tileScale = 2.0;
	int mapNumColumns = 25;
	int mapNumRows = 20;

	std::fstream mapFile;
	mapFile.open("assets/tilemaps/jungle.map");
	
	// Check if the file is open.
	if (!mapFile.is_open()) {
		spdlog::error("Error opening the map file. Please check file path.");
		return;
	}
	
	// Loop through the map file and create the tiles.
	for (int row = 0; row < 20; row++) {
		for (int column = 0; column < 25; column++) {

			char ch[2] = { 0, 0 };

			// Read the next character from the file.
			mapFile.get(ch[0]);

			// Convert the char to an int and multiply it by the tile size.
			int srcRectY = std::atoi(&ch[0]) * tileSize;
			mapFile.get(ch[0]);
			int srcRectX = std::atoi(&ch[0]) * tileSize;

			// Ignore the comma and the space.
			mapFile.ignore();

			// Create an entity for each tile.
			Entity tile = manager->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(column * (tileSize * tileScale), row * (tileSize * tileScale)), glm::vec2(tileScale, tileScale), 0.0);
			tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, false,srcRectX, srcRectY);

		}
	}
	
	mapFile.close();
	mapWidth = mapNumColumns * tileSize * tileScale;
	mapHeight = mapNumRows * tileSize * tileScale;

	// Creating the entities.
	Entity tank = manager->CreateEntity();
	tank.AddComponent<TransformComponent>(glm::vec2(500.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);
	tank.AddComponent<BoxColliderComponent>(32, 32);
	tank.AddComponent<ProjectileComponent>(glm::vec2(0.0, 100.0), 2000, 10000, 0, false);
	tank.AddComponent<HealthComponent>(100);


	Entity chopper = manager->CreateEntity();
	chopper.AddComponent<TransformComponent>(glm::vec2(10.0, 50.0), glm::vec2(1.0, 1.0), 0.0);
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 2, false);
	chopper.AddComponent<AnimationComponent>(2, 10, true);
	chopper.AddComponent<KeyboardControllerComponent>(glm::vec2(0, -90), glm::vec2(0, 90), glm::vec2(-90, 0), glm::vec2(90, 0));
	chopper.AddComponent<CameraComponent>();
	chopper.AddComponent<HealthComponent>(100);
	chopper.AddComponent<ProjectileComponent>(glm::vec2(150.0, 150.0), 0, 10000, 0, true);

	Entity radar = manager->CreateEntity();
	radar.AddComponent<TransformComponent>(glm::vec2(windowWidth - 74, 10.0), glm::vec2(1.0, 1.0), 0.0);
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 1, true);
	radar.AddComponent<AnimationComponent>(8, 5, true);
	radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));

	Entity truck = manager->CreateEntity();
	truck.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 2);
	truck.AddComponent<BoxColliderComponent>(32, 32);
	truck.AddComponent<ProjectileComponent>(glm::vec2(0.0, 100.0), 2000, 10000, 0, false);
	truck.AddComponent<HealthComponent>(100);
	
	// TODO: Add an error message that pops up if the file is not found.
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
	 
	 // Reset all handlers for the current frame.
	 eventBus->Reset();

	 // Perform the subscription of events for all systems.
	 manager->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
	 manager->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventBus);
	 manager->GetSystem<ProjectileSystem>().SubscribeToEvents(eventBus);

	// Invoke all the systems in the manager.
	manager->Update();

	// Updating game objects.
	manager->GetSystem<MovementSystem>().Update(deltaTime);
	manager->GetSystem<CollisionSystem>().Update(eventBus);
	manager->GetSystem<KeyboardControlSystem>().Update();
	manager->GetSystem<CameraMovementSystem>().Update(camera);
	manager->GetSystem<ProjectileSystem>().Update(manager);
	manager->GetSystem<LifespanSystem>().Update();
}

void Engine::Render() {
	// Set background color to red.
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);

	// Clear the back buffer.
	SDL_RenderClear(renderer);

	// Invoking the systems needed to render the game.
	manager->GetSystem<RenderSystem>().Update(renderer, assetManager, camera);
	manager->GetSystem<AnimationSystem>().Update();

	// If debug mode is set to true, then the collision shapes will be rendered.
	if (isDebugMode) {
		manager->GetSystem<RenderCollisionSystem>().Update(renderer, camera);
	}

	// Show the back buffer.
	SDL_RenderPresent(renderer);
}

void Engine::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}



