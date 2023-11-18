#pragma once

#include "../src/ECS/ECS.h"

class MovemenSystem : public System {
public:
	MovemenSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<RigidbodyComponent>();
	}


};