#pragma once

#include "../src/ECS/ECS.h"
#include "../src/Events/Events.h"
#include "../src/EventBus/EventBus.h"

class KeyboardMovementSystem : public System {
public:
	KeyboardMovementSystem() {
		RequireComponent<TransformComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<KeyboardPressedEvent>(this, &KeyboardMovementSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyboardPressedEvent& event) {
		spdlog::info("Key Pressed: {}", event.keyCode);
	}

	void Update() {
		
	}
};