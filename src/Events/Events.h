#pragma once

#include "../src/ECS/ECS.h"
#include "../src/EventBus/Event.h"
#include <SDL.h>

class CollisionEvent : public Event {
public:
	Entity a;
	Entity b;
	CollisionEvent(Entity a, Entity b) : a(a), b(b) {}
};


class KeyboardPressedEvent : public Event {
public:
	SDL_Keycode symbol;
	KeyboardPressedEvent(SDL_Keycode keyCode) : symbol(keyCode) {}
};
