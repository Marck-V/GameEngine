#include "ECS.h"
#include <spdlog/spdlog.h>


int IComponent::nextID = 0;

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
	entity.manager = this;
	entitiesToCreate.insert(entity);

	// Check that the signature vector has enough space for the new entity
	if (entityID >= entityComponentSignatures.size()) {
		entityComponentSignatures.resize(entityID + 1);
	}

	spdlog::info("Entity created with ID: {0}", entityID);

	return entity;
}

void Manager::AddEntityToSystems(Entity entity) {
	const auto entityID = entity.GetID();

	const auto entityComponentSignature = entityComponentSignatures[entityID];
	
	// Loop all the systems
	for (auto& system : systems) {
		const auto& systemComponentSignature = system.second->GetComponentSignature();

		// Check if the entity has the required components to be added to the system
		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

		if (isInterested) {
			system.second->AddEntity(entity);
		}
	}
}
void Manager::Update()
{
	// Add entities that are waiting to be created to the active systems
	for (auto entity : entitiesToCreate) {
		AddEntityToSystems(entity);
	}

	entitiesToCreate.clear();

}
