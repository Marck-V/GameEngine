#pragma once

#include "src/ECS/ECS.h"
#include "src/Components/Components.h"

void SetEntityPosition(Entity entity, double x, double y) {
	if (entity.HasComponent<TransformComponent>()) {
		auto& transform = entity.GetComponent<TransformComponent>();
		transform.position.x = x;
		transform.position.y = y;
	}
	else {
		spdlog::error("Entity does not have a TransformComponent!");
	}
}

class ScriptSystem : public System {
public:
	ScriptSystem() {
		RequireComponent<ScriptComponent>();
	}

	void CreateLuaBindings(sol::state& lua) {
		// Create the entity usertype so lua knows what an entity is.
		lua.new_usertype<Entity>(
			"entity",
			"get_id", &Entity::GetID,
			"kill", &Entity::Kill,
			"has_tag", &Entity::HasTag,
			"is_in_group", &Entity::IsInGroup
		);

		// Create all the bindings between C++ and Lua.
		lua.set_function("set_position", SetEntityPosition);
	}

	void Update(double deltaTime, int ellapsedTime) {
		// Loops through all entities that have a script component and calls the function
		for (auto entity : GetSystemEntities()) {
			const auto script = entity.GetComponent<ScriptComponent>();
			script.func(entity, deltaTime, ellapsedTime);
		}
	}
};