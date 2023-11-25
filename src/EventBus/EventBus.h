#pragma once

#include <spdlog/spdlog.h>
#include <map>
#include <typeindex>
#include "../src/EventBus/Event.h"



class IEventCallback {
public:
	virtual ~IEventCallback() = default;
	
	void Execute(Event& e) {
		Call(e);
	}
private:
	virtual void Call(Event e) = 0;
};

class CallbackFunction;
template <typename TOwner ,typename TEvent>
class EventCallback : public IEventCallback {
public:
	EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction) {
		this->ownerInstance = ownerInstance;
		this->callbackFunction = callbackFunction;
	}

	~EventCallback() = default;
private:

	typedef void(TOwner::*CallbackFunction)(TEvent&);

	TOwner* ownerInstance;
	CallbackFunction callbackFunction;

	virtual void Call(Event& e) override {
		// Call the function.
		std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
	}
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {


public:
	EventBus() {
		spdlog::info("EventBus created.");
	}
	~EventBus() {
		spdlog::info("EventBus destroyed.");
	}

	
	// Subscribe to an event of type T.
	template <typename TEvent, typename TOwner>
	void SubscribeToEvent(TOwner* ownerInstance, void(TOwner::* callbackFunction)(TEvent&)) {
		// Check if the event type is already in the map.
		if (!subscribers[typeid(TEvent)].get()) {
			subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
		}

		// Create a new subscriber and add it to the list.
		auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
		subscribers[typeid(TEvent)].push_back(std::move(subscriber));
	}


	// Emit an event of type T.
	template <typename TEvent, typename ...TArgs>
	void EmitEvent(TArgs&& ...args) {
		auto handlers = subscribers[typeid(TEvent)].get();
		if (handlers) {
			for (auto it = handlers->begin(); it != handlers->end(); it++) {
				auto handler = it->get();
				TEvent event(std::forward<TArgs>(args)...);
				handler->Execute(event);
			}	
		}
		spdlog::info("Event emitted.");
	}

private:
	// Map of event types to a vector of subscribers.
	std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;
};
