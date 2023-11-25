#pragma once

#include "../src/ECS/ECS.h"

class Event {

};

class CollisionEvent : Event {
public:
	Entity entityA;
	Entity entityB;
	CollisionEvent(Entity entityA, Entity entityB) : entityA(entityA), entityB(entityB) {};
};