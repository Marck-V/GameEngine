#include "LevelLoader.h"
//#include "src/Components/Components.h"
//#include "src/Engine/Engine.h"
#include "Components/Components.h"
#include "Engine/Engine.h"
#include <fstream>

LevelLoader::LevelLoader()
{
	spdlog::info("LevelLoader Constructor Called.");
}

LevelLoader::~LevelLoader()
{
	spdlog::info("LevelLoader Destructor Called.");
}

void LevelLoader::LoadLevel(sol::state& lua, const std::unique_ptr<Manager>& manager, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, int levelNumber)
{
	sol::load_result script = lua.load_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");
	if (!script.valid()) {
		sol::error err = script;
		std::string errorMsg = err.what();
		spdlog::error("Error loading lua script: " + errorMsg);
		return;
	}
	// Load the level file.
	lua.script_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");

	// Read the table for the current level.
	sol::table level =	lua["Level"];

	// Read the level assets.
	sol::table assets = level["assets"];

	int i = 0;
	while (true) {
		sol::optional<sol::table> hasAsset = assets[i];

		if (hasAsset == sol::nullopt) {
			break;
		}

		sol::table asset = assets[i];
		std::string assetType = asset["type"];
		std::string assetID = asset["id"];

		if (assetType == "texture") {
			assetManager->AddTexture(renderer, assetID, asset["file"]);
			spdlog::info("Added texture: " + assetID);
		}

		if (assetType == "font") {
			assetManager->AddFont(assetID, asset["file"], asset["font_size"]);
			spdlog::info("Added font: " + assetID);
		}

		i++;
	}

	// Tile map.
	sol::table map = level["tilemap"];
	std::string mapFilePath = map["map_file"];
	std::string mapTextureAssetId = map["texture_asset_id"];
	int mapNumRows = map["num_rows"];
	int mapNumCols = map["num_cols"];
	int tileSize = map["tile_size"];
	double mapScale = map["scale"];
	std::fstream mapFile;
	mapFile.open(mapFilePath);
	for (int y = 0; y < mapNumRows; y++) {
		for (int x = 0; x < mapNumCols; x++) {
			char ch;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.ignore();

			Entity tile = manager->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(x * (mapScale * tileSize), y * (mapScale * tileSize)), glm::vec2(mapScale, mapScale), 0.0);
			tile.AddComponent<SpriteComponent>(mapTextureAssetId, tileSize, tileSize, 0, false, srcRectX, srcRectY);
		}
	}
	mapFile.close();
	Engine::mapWidth = mapNumCols * tileSize * mapScale;
	Engine::mapHeight = mapNumRows * tileSize * mapScale;

	////////////////////////////////////////////////////////////////////////////
	// Read the level entities and their components
	////////////////////////////////////////////////////////////////////////////
	sol::table entities = level["entities"];
	i = 0;
	while (true) {
		sol::optional<sol::table> hasEntity = entities[i];
		if (hasEntity == sol::nullopt) {
			break;
		}

		sol::table entity = entities[i];

		Entity newEntity = manager->CreateEntity();

		// Tag
		sol::optional<std::string> tag = entity["tag"];
		if (tag != sol::nullopt) {
			newEntity.Tag(entity["tag"]);
		}

		// Group
		sol::optional<std::string> group = entity["group"];
		if (group != sol::nullopt) {
			newEntity.Group(entity["group"]);
		}

		// Components
		sol::optional<sol::table> hasComponents = entity["components"];
		if (hasComponents != sol::nullopt) {
			// Transform
			sol::optional<sol::table> transform = entity["components"]["transform"];
			if (transform != sol::nullopt) {
				newEntity.AddComponent<TransformComponent>(
					glm::vec2(
						entity["components"]["transform"]["position"]["x"],
						entity["components"]["transform"]["position"]["y"]
					),
					glm::vec2(
						entity["components"]["transform"]["scale"]["x"].get_or(1.0),
						entity["components"]["transform"]["scale"]["y"].get_or(1.0)
					),
					entity["components"]["transform"]["rotation"].get_or(0.0)
				);
			}

			// RigidBody
			sol::optional<sol::table> rigidbody = entity["components"]["rigidbody"];
			if (rigidbody != sol::nullopt) {
				newEntity.AddComponent<RigidBodyComponent>(
					glm::vec2(
						entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
						entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)
					)
				);
			}

			// Sprite
			sol::optional<sol::table> sprite = entity["components"]["sprite"];
			if (sprite != sol::nullopt) {
				newEntity.AddComponent<SpriteComponent>(
					entity["components"]["sprite"]["texture_asset_id"],
					entity["components"]["sprite"]["width"],
					entity["components"]["sprite"]["height"],
					entity["components"]["sprite"]["z_index"].get_or(1),
					entity["components"]["sprite"]["fixed"].get_or(false),
					entity["components"]["sprite"]["src_rect_x"].get_or(0),
					entity["components"]["sprite"]["src_rect_y"].get_or(0)
				);
			}

			// Animation
			sol::optional<sol::table> animation = entity["components"]["animation"];
			if (animation != sol::nullopt) {
				newEntity.AddComponent<AnimationComponent>(
					entity["components"]["animation"]["num_frames"].get_or(1),
					entity["components"]["animation"]["speed_rate"].get_or(1)
				);
			}

			// BoxCollider
			sol::optional<sol::table> collider = entity["components"]["boxcollider"];
			if (collider != sol::nullopt) {
				newEntity.AddComponent<BoxColliderComponent>(
					entity["components"]["boxcollider"]["width"],
					entity["components"]["boxcollider"]["height"],
					glm::vec2(
						entity["components"]["boxcollider"]["offset"]["x"].get_or(0),
						entity["components"]["boxcollider"]["offset"]["y"].get_or(0)
					)
				);
			}

			// Health
			sol::optional<sol::table> health = entity["components"]["health"];
			if (health != sol::nullopt) {
				newEntity.AddComponent<HealthComponent>(
					static_cast<int>(entity["components"]["health"]["health_percentage"].get_or(100))
				);
			}

			// ProjectileEmitter
			sol::optional<sol::table> projectileEmitter = entity["components"]["projectile_emitter"];
			if (projectileEmitter != sol::nullopt) {
				newEntity.AddComponent<ProjectileEmitterComponent>(
					glm::vec2(
						entity["components"]["projectile_emitter"]["projectile_velocity"]["x"],
						entity["components"]["projectile_emitter"]["projectile_velocity"]["y"]
					),
					static_cast<int>(entity["components"]["projectile_emitter"]["repeat_frequency"].get_or(1)) * 1000,
					static_cast<int>(entity["components"]["projectile_emitter"]["projectile_duration"].get_or(10)) * 1000,
					static_cast<int>(entity["components"]["projectile_emitter"]["hit_percentage_damage"].get_or(10)),
					entity["components"]["projectile_emitter"]["friendly"].get_or(false)
				);
			}

			// Camera
			sol::optional<sol::table> cameraFollow = entity["components"]["camera_follow"];
			if (cameraFollow != sol::nullopt) {
				newEntity.AddComponent<CameraComponent>();
			}

			// KeyboardController
			sol::optional<sol::table> keyboardControlled = entity["components"]["keyboard_controller"];
			if (keyboardControlled != sol::nullopt) {
				newEntity.AddComponent<KeyboardControllerComponent>(
					glm::vec2(
						entity["components"]["keyboard_controller"]["up_velocity"]["x"],
						entity["components"]["keyboard_controller"]["up_velocity"]["y"]
					),
					glm::vec2(
						entity["components"]["keyboard_controller"]["right_velocity"]["x"],
						entity["components"]["keyboard_controller"]["right_velocity"]["y"]
					),
					glm::vec2(
						entity["components"]["keyboard_controller"]["down_velocity"]["x"],
						entity["components"]["keyboard_controller"]["down_velocity"]["y"]
					),
					glm::vec2(
						entity["components"]["keyboard_controller"]["left_velocity"]["x"],
						entity["components"]["keyboard_controller"]["left_velocity"]["y"]
					)
				);
			}

			// Script
			sol::optional<sol::table> script = entity["components"]["on_update_script"];
			if (script != sol::nullopt) {
				sol::function func = entity["components"]["on_update_script"][0];
				newEntity.AddComponent<ScriptComponent>(func);
			}
		}
		i++;
	}


	//// Adding the textures to the asset container.
	assetManager->AddTexture(renderer, "tank-image", "assets/images/tank-panther-right.png");
	assetManager->AddTexture(renderer, "truck-image", "assets/images/truck-ford-right.png");
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
