#include "ECS.h"
#include <spdlog/spdlog.h>

int Entity::GetID() const
{
	return id;
	
}

void System::AddEntity(Entity entity)
{
	entities.push_back(entity);
}

void System::RemoveEntity(Entity entity)
{
	// Modern C++ way of iterating through a vector and removing an element. Using a lambda function.
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity e) {return e == entity; }), entities.end());
}

std::vector<Entity> System::GetSystemEntities() const
{
	return entities;
}

const Signature& System::GetComponentSignature() const
{
	// TODO: insert return statement here
	return componentSignature;
}

Entity Manager::CreateEntity()
{
	int entityID = 0;
	
	entityID = numEntities++;

	Entity entity(entityID);
	entitiesToCreate.insert(entity);

	spdlog::info("Entity created with ID: {0}", entityID);

	return entity;
}

void Manager::Update()
{

}
