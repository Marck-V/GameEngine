#include "LevelLoader.h"
#include "src/Components/Components.h"
#include "src/Engine/Engine.h"
#include <fstream>

LevelLoader::LevelLoader()
{
	spdlog::info("LevelLoader Constructor Called.");
}

LevelLoader::~LevelLoader()
{
	spdlog::info("LevelLoader Destructor Called.");
}

void LevelLoader::LoadLevel(const std::unique_ptr<Manager>& manager, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, int level)
{
	//// Adding the textures to the asset container.
	//assetManager->AddTexture(renderer, "tank-image", "assets/images/tank-panther-right.png");
	//assetManager->AddTexture(renderer, "truck-image", "assets/images/truck-ford-right.png");
	//assetManager->AddTexture(renderer, "tilemap-image", "assets/tilemaps/jungle.png");
	//assetManager->AddTexture(renderer, "chopper-image", "assets/images/chopper-spritesheet.png");
	//assetManager->AddTexture(renderer, "radar-image", "assets/images/radar.png");
	//assetManager->AddTexture(renderer, "bullet-image", "assets/images/bullet.png");
	//assetManager->AddTexture(renderer, "tree-image", "assets/images/tree.png");
	//assetManager->AddFont("charriot-font", "assets/fonts/charriot.ttf", 20);
	//// TODO: See if this can be improved by using a 2D array.

	//// Load the tilemap
	//int tileSize = 32;
	//double tileScale = 2.0;
	//int mapNumColumns = 25;
	//int mapNumRows = 20;

	//std::fstream mapFile;
	//mapFile.open("assets/tilemaps/jungle.map");

	//// Check if the file is open.
	//if (!mapFile.is_open()) {
	//	spdlog::error("Error opening the map file. Please check file path.");
	//	return;
	//}

	//// Loop through the map file and create the tiles.
	//for (int row = 0; row < 20; row++) {
	//	for (int column = 0; column < 25; column++) {

	//		char ch[2] = { 0, 0 };

	//		// Read the next character from the file.
	//		mapFile.get(ch[0]);

	//		// Convert the char to an int and multiply it by the tile size.
	//		int srcRectY = std::atoi(&ch[0]) * tileSize;
	//		mapFile.get(ch[0]);
	//		int srcRectX = std::atoi(&ch[0]) * tileSize;

	//		// Ignore the comma and the space.
	//		mapFile.ignore();

	//		// Create an entity for each tile.
	//		Entity tile = manager->CreateEntity();
	//		tile.AddComponent<TransformComponent>(glm::vec2(column * (tileSize * tileScale), row * (tileSize * tileScale)), glm::vec2(tileScale, tileScale), 0.0);
	//		tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, false, srcRectX, srcRectY);

	//	}
	//}

	//mapFile.close();
	//Engine::mapWidth = mapNumColumns * tileSize * tileScale;
	//Engine::mapHeight = mapNumRows * tileSize * tileScale;

	//// Creating the entities.
	//Entity tank = manager->CreateEntity();
	//tank.Group("enemies");
	//tank.AddComponent<TransformComponent>(glm::vec2(500.0, 500.0), glm::vec2(1.0, 1.0), 0.0);
	//tank.AddComponent<RigidBodyComponent>(glm::vec2(30.0, 0.0));
	//tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);
	//tank.AddComponent<BoxColliderComponent>(25, 18, glm::vec2(5, 7));
	//tank.AddComponent<HealthComponent>(100);

	//Entity chopper = manager->CreateEntity();
	//chopper.Tag("player");
	//chopper.AddComponent<TransformComponent>(glm::vec2(10.0, 50.0), glm::vec2(1.0, 1.0), 0.0);
	//chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	//chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1, false);
	//chopper.AddComponent<AnimationComponent>(2, 10, true);
	//chopper.AddComponent<KeyboardControllerComponent>(glm::vec2(0, -100), glm::vec2(0, 100), glm::vec2(-100, 0), glm::vec2(100, 0));
	//chopper.AddComponent<CameraComponent>();
	//chopper.AddComponent<BoxColliderComponent>(32, 32);
	//chopper.AddComponent<HealthComponent>(100);
	//chopper.AddComponent<ProjectileEmitterComponent>(glm::vec2(150.0, 150.0), 0, 10000, 10, true);

	//Entity radar = manager->CreateEntity();
	//radar.AddComponent<TransformComponent>(glm::vec2(Engine::windowWidth - 74, 10.0), glm::vec2(1.0, 1.0), 0.0);
	//radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 1, true);
	//radar.AddComponent<AnimationComponent>(8, 5, true);
	//radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));

	//Entity truck = manager->CreateEntity();
	//truck.Group("enemies");
	//truck.AddComponent<TransformComponent>(glm::vec2(300.0, 300.0), glm::vec2(1.0, 1.0), 0.0);
	//truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	//truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 2);
	//truck.AddComponent<BoxColliderComponent>(32, 32);
	//truck.AddComponent<ProjectileEmitterComponent>(glm::vec2(0.0, 100.0), 2000, 5000, 10, false);
	//truck.AddComponent<HealthComponent>(100);

	//Entity treeA = manager->CreateEntity();
	//treeA.Group("obstacles");
	//treeA.AddComponent<TransformComponent>(glm::vec2(600.0, 495.0), glm::vec2(1.0, 1.0), 0.0);
	//treeA.AddComponent<SpriteComponent>("tree-image", 16, 32, 2);
	//treeA.AddComponent<BoxColliderComponent>(16, 32);

	//Entity treeB = manager->CreateEntity();
	//treeB.Group("obstacles");
	//treeB.AddComponent<TransformComponent>(glm::vec2(400.0, 495.0), glm::vec2(1.0, 1.0), 0.0);
	//treeB.AddComponent<SpriteComponent>("tree-image", 16, 32, 2);
	//treeB.AddComponent<BoxColliderComponent>(16, 32);

	//Entity label = manager->CreateEntity();
	//SDL_Color white = { 255, 255, 255, 255 };
	//label.AddComponent<TextLabelComponent>(glm::vec2(Engine::windowWidth / 2 - 50, 10), "Chopper 1.0", "charriot-font", white, true);
}
