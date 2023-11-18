#pragma once

#include <glm/glm.hpp>

struct RigidBodyComponent {
	glm::vec2 velocity;

	RigidBodyComponent(glm::vec2 velocity = glm::vec2(0.0f, 0.0f)) {
		this->velocity = velocity;
	}
};

struct TransformComponent {
	glm::vec2 position;
	glm::vec2 scale;
	double rotation;

	TransformComponent(glm::vec2 position = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1, 1), double rotation = 0.0) {
		this->position = position;
		this->scale = scale;
		this->rotation = rotation;
	};
};

struct SpriteComponent {
	int width;
	int height;

	SpriteComponent(int width = 0, int height = 0) {
		this->width = width;
		this->height = height;
	}

};