#include "Engine.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <imgui/imgui_impl_sdl.h>
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
#include "../Systems/RenderTextSystem.h"
#include "../Systems/RenderHealthBarSystem.h"
#include "../Systems/RenderGUISystem.h"
#include "../src/Events/Events.h"
#include "../src/EventBus/EventBus.h"
#include "LevelLoader.h"

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
	
	if (TTF_Init() != 0) {
		spdlog::error("Error initializing SDL TTF.");
		return;
	}

	SDL_DisplayMode displayMode;

	// Get the current display mode and resolution of the first display.
	SDL_GetCurrentDisplayMode(0, &displayMode);

	windowWidth = 1600;
	windowHeight = 900;



	window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_RENDERER_ACCELERATED);

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

	// Initialize ImGui
	ImGui::CreateContext();
	ImGuiSDL::Initialize(renderer, windowWidth, windowHeight);

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

		// ImGUI SDL Input handling.
		ImGui_ImplSDL2_ProcessEvent(&event);
		ImGuiIO& io = ImGui::GetIO();

		int mouseX, mouseY;
		const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

		io.MousePos = ImVec2(mouseX, mouseY);
		io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

		// Handling core sdl events.
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

void Engine::Setup() {
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
	manager->AddSystem<RenderTextSystem>();
	manager->AddSystem<RenderHealthBarSystem>();
	manager->AddSystem<RenderGUISystem>();

	LevelLoader loader;
	loader.LoadLevel(manager, assetManager, renderer, 1);
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
	 manager->GetSystem<MovementSystem>().SubscribeToEvents(eventBus);
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
	manager->GetSystem<RenderTextSystem>().Update(renderer, assetManager, camera);
	manager->GetSystem<RenderHealthBarSystem>().Update(renderer, assetManager, camera);

	// If debug mode is set to true, then the collision shapes will be rendered.
	if (isDebugMode) {
		manager->GetSystem<RenderCollisionSystem>().Update(renderer, camera);

		manager->GetSystem<RenderGUISystem>().Update(manager, camera);
	}

	// Show the back buffer.
	SDL_RenderPresent(renderer);
}

void Engine::Destroy() {
	ImGuiSDL::Deinitialize();
	ImGui::DestroyContext();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}