#pragma once

#include "../src/ECS/ECS.h"
#include "../src/Events/Events.h"
#include "../src/EventBus/EventBus.h"
#include <string>

class KeyboardControlSystem : public System {
public:
	KeyboardControlSystem() {
		
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<KeyboardPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyboardPressedEvent& event) {
		std::string keyCode = std::to_string(event.keyCode);
		std::string keySymbol(1, event.keyCode);
		spdlog::info("Key Pressed: {}", keySymbol);
	}

	void Update() {
		
	}
};