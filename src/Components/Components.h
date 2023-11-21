#pragma once

#include <glm/glm.hpp>
#include <SDL.h>

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

	// Default constructor (Position: 0, 0, Scale: 1, 1, Rotation: 0)
	TransformComponent(glm::vec2 position = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1, 1), double rotation = 0.0) {
		this->position = position;
		this->scale = scale;
		this->rotation = rotation;
	};
};

struct SpriteComponent {
	std::string assetID;
	int width;
	int height;
	int zIndex;
	SDL_Rect srcRect;

	// TODO: Add layers
	SpriteComponent(std::string assetID = "", int width = 0, int height = 0, int zIndex = 0 ,int srcRectX = 0, int srcRectY = 0) {
		this->assetID = assetID;
		this->width = width;
		this->height = height;
		this->zIndex = zIndex;
		this->srcRect = { srcRectX, srcRectY, width, height };
	}

};

struct AnimationComponent {
	int numFrames;
	int currentFrame;
	int frameRateSpeed;
	bool isLoop;
	int startTime;

	AnimationComponent(int numFrames = 1, int frameRateSpeed = 1, bool isLoop = true) {
		this->numFrames = numFrames;
		this->currentFrame = currentFrame;
		this->frameRateSpeed = frameRateSpeed;
		this->isLoop = isLoop;
		this->startTime = SDL_GetTicks();
	}
};

struct BoxColliderComponent {
	int width;
	int height;
	glm::vec2 offset;

	BoxColliderComponent(int width = 0, int height = 0, glm::vec2 offset = glm::vec2(0)) {
		this->width = width;
		this->height = height;
		this->offset = offset;
	}
};