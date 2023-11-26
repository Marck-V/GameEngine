#pragma once

#include "../src/ECS/ECS.h"
#include "../src/EventBus/Event.h"
#include <SDL.h>

class CollisionEvent : public Event {
public:
	Entity entityA;
	Entity entityB;
	CollisionEvent(Entity entityA, Entity entityB) : entityA(entityA), entityB(entityB) {}
};


class KeyboardPressedEvent : public Event {
public:
	SDL_Keycode keyCode;
	KeyboardPressedEvent(SDL_Keycode keyCode) : keyCode(keyCode) {}
};