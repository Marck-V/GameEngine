#pragma once

#include "../src/ECS/ECS.h"
#include "../src/EventBus/Event.h"

class CollisionEvent : Event {
public:
	Entity entityA;
	Entity entityB;
	CollisionEvent(Entity entityA, Entity entityB) : entityA(entityA), entityB(entityB) {};
};