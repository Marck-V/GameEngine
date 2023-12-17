#pragma once

#include "../src/ECS/ECS.h"
#include "../src/Events/Events.h"
#include "../src/EventBus/EventBus.h"
#include <string>

class KeyboardControlSystem : public System {
public:
	KeyboardControlSystem() {
		RequireComponent<KeyboardControllerComponent>();
		RequireComponent<RigidBodyComponent>();
		RequireComponent<SpriteComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<KeyboardPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyboardPressedEvent& event) {
		for (auto entity : GetSystemEntities()) {
			const auto keyboardControl = entity.GetComponent<KeyboardControllerComponent>();
			auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>(); 

			switch (event.symbol) {
			case SDLK_UP:
				rigidBody.velocity = keyboardControl.upVel;
				sprite.srcRect.y = sprite.height * 0;
				break;

			case SDLK_RIGHT:
				rigidBody.velocity = keyboardControl.rightVel;
				sprite.srcRect.y = sprite.height * 1;
				break;

			case SDLK_DOWN:
				rigidBody.velocity = keyboardControl.downVel;
				sprite.srcRect.y = sprite.height * 2;
				break;

			case SDLK_LEFT:
				rigidBody.velocity = keyboardControl.leftVel;
				sprite.srcRect.y = sprite.height * 3;
				break;

			case SDLK_SPACE:
				
				break;
			}
		}
	}

	void Update() {
		
	}
};