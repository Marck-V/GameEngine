#pragma once

#include <spdlog/spdlog.h>
#include <map>
#include <typeindex>

class EventCallback {

};

typedef std::list<std::unique_ptr<EventCallback>> HandlerList;

class EventBus {


public:
	EventBus() {
		spdlog::info("EventBus created.");
	}
	~EventBus() {
		spdlog::info("EventBus destroyed.");
	}

	// Subscribe to an event of type T.
	void SubscribeToEvent() {
		spdlog::info("Subscribed to event.");
	}

	// Emit an event of type T.
	void EmitEvent() {
		spdlog::info("Event emitted.");
	}

private:
	// Map of event types to a vector of subscribers.
	std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;
};
