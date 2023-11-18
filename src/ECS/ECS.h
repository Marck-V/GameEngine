#pragma once

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <memory>
#include <spdlog/spdlog.h>

const unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;


class ECS
{

};

class Manager;

class Entity {

private:

		int id;

public:
		Entity(int id) : id(id) {};

		int GetID() const;

		Entity& operator=(const Entity& other) = default;
			
		bool operator==(const Entity& other) const { return id == other.id; }
		bool operator !=(const Entity& other) const { return id != other.id; }
		bool operator >(const Entity& other) const { return id > other.id; }
		bool operator <(const Entity& other) const { return id < other.id; }

		template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
		template <typename TComponent> void RemoveComponent();
		template <typename TComponent> bool HasComponent();
		template <typename TComponent> TComponent& GetComponent() const;

		// Hold a pointer to the entity's owner manager.
		Manager* manager;
};

class System {
private:
		Signature componentSignature;

		std::vector<Entity> entities;

public:
	System() = default;
	~System() = default;

	void AddEntity(Entity entity);
	void RemoveEntity(Entity entity);

	std::vector<Entity> GetSystemEntities() const;

	const Signature& GetComponentSignature() const;

	// Defines the component type that entities must have to be added to the system
	template <typename T> void RequireComponent();
};

struct IComponent {
	protected:
		static int nextID;
};

// Used to assign a unique IDS to each component type.
template<typename TComponent> 
class Component : public IComponent{
public:
	// Returns the unique ID for the component type.
	static int GetID(){
		static auto id = nextID++;
		return id;
	}
		
};

template <typename T> void System::RequireComponent() {
	const auto componentID = Component<T>::GetID();
	componentSignature.set(componentID);
}


class IPool {
public:
	virtual ~IPool() {};

};


template <typename T>
class Pool : public IPool {
private:
	std::vector<T> data;

public:
	Pool(int size = 100) {
		data.resize(size);
	}
	virtual ~Pool() = default;

	bool isEmpty() const {
		return data.empty();
	}

	int GetSize() const {
		return data.size();
	}

	void Resize(int size) {
		data.resize(size);
	}

	void Clear() {
		data.clear();
	}

	void Add(T object) {
		data.push_back(object);
	}


	void Set(int index, T object) {
		data[index] = object;
	}

	T& Get(int index) {
		
		return static_cast<T&>(data[index]);
	}

	T& operator [](unsigned int index) {
		return data[index];
	}

};


// The manager is going to be the class that handles the creation and destruction of entities and components.
class Manager {
private:
	int numEntities = 0;

	// A vector of componenet pools. Each pool is going to comntain the components of a specific type.
	// The index of the pool in the vector is going to be the ID of the component type.
	// This way we can access the pool of a specific component type by using the ID of the component type.
	std::vector<std::shared_ptr<IPool>> componentPools;

	// A vector of signatures. Each signature is going to represent the component types that an entity has.
	// The index of the signature in the vector is going to be the ID of the entity.
	std::vector<Signature> entityComponentSignatures;

	// An unordered map of systems.
	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

	// Set of entities that are going to be added or destroyed at the end of the frame.
	std::set<Entity> entitiesToDestroy;
	std::set<Entity> entitiesToCreate;

	
	
public:

	Manager() = default;

	void Update();

	// Entity management
	Entity CreateEntity();
	
	// Component management
	template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
	template <typename TComponent> void RemoveComponent(Entity entity);
	template <typename TComponent> bool HasComponent(Entity entity);
	template <typename TComponent> TComponent& GetComponent(Entity entity) const;

	template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
	template <typename TSystem> void RemoveSystem();
	template <typename TSystem> bool HasSystem() const;
	template <typename TSystem> TSystem& GetSystem() const;

	// Checks the component signarures of the entity and adds it to the systems that require the components that the entity has.
	void AddEntityToSystems(Entity entity);
};


template <typename TComponent, typename... TArgs>
void Manager::AddComponent(Entity entity, TArgs&& ... args) {

	const auto componentID = Component<TComponent>::GetID();
	const auto entityID = entity.GetID();

	// If you have a new component type, add it to the component pools by resizing the vector.
	if(componentID >= componentPools.size()){
		componentPools.resize(componentID + 1, nullptr);
	}

	// If the component pool is empty, create it.
	if (!componentPools[componentID]) {
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		componentPools[componentID] = newComponentPool;
	}

	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentID]);
	
	if(entityID >= componentPool->GetSize()) {
		componentPool->Resize(entityID + 1);
	}

	TComponent newComponent(std::forward<TArgs>(args)...);

	componentPool->Set(entityID, newComponent);

	entityComponentSignatures[entityID].set(componentID);

	spdlog::info("Component ID: " + std::to_string(componentID)+ " was added to entity ID: " + std::to_string(entityID));
};

template <typename TComponent>
void Manager::RemoveComponent(Entity entity) {
	const auto componentID = Component<TComponent>::GetID();
	const auto entityID = entity.GetID();

	entityComponentSignatures[entityID].set(componentID, false);

	spdlog::info("Component ID: " + std::to_string(componentID) + " was removed from entity ID: " + std::to_string(entityID));
};

template <typename TComponent>
bool Manager::HasComponent(Entity entity) {
	const auto componentID = Component<TComponent>::GetID();
	const auto entityID = entity.GetID();

	return entityComponentSignatures[entityID].test(componentID);
};

template <typename TComponent>
TComponent& Manager::GetComponent(Entity entity) const {
	const auto componentID = Component<TComponent>::GetID();
	const auto entityID = entity.GetID();

	auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentID]);

	return componentPool->Get(entityID);
}

template <typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs&& ...args) {
	manager->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
};

template <typename TComponent>
void Entity::RemoveComponent() {
	manager->RemoveComponent<TComponent>(*this);
};

template <typename TComponent>
bool Entity::HasComponent() {
	return manager->HasComponent<TComponent>(*this);
};

template <typename TComponent>
TComponent& Entity::GetComponent() const {
	return manager->GetComponent<TComponent>(*this);
};

template <typename TSystem, typename ...TArgs> 
void Manager::AddSystem(TArgs&& ...args) {
	std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
	systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
};

template <typename TSystem> 
void Manager::RemoveSystem() {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	systems.erase(system);
}

template <typename TSystem>
bool Manager::HasSystem() const {
	return system = systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem> 
TSystem& Manager::GetSystem() const {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	
	return *(std::static_pointer_cast<TSystem>(system->second));

	
}