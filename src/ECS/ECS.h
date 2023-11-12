#pragma once

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>

const unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;


class ECS
{

};

class Entity {

private:

		int id;

public:
		Entity(int id) : id(id) {};

		int GetID() const;

		Entity& operator=(const Entity& other) = default;
			
		bool operator==(const Entity& other) const {
			return id == other.id;
		}
		
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
template<typename T> 
class Component : public IComponent{
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
		data.push_back(component);
	}


	void Set(int entityID, T object) {
		data[index] = component;
	}

	T& Get(int entityID) {
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
	std::vector<IPool*> componentPools;

	// A vector of signatures. Each signature is going to represent the component types that an entity has.
	// The index of the signature in the vector is going to be the ID of the entity.
	std::vector<Signature> entityComponentSignatures;

	// An unordered map of systems.
	std::unordered_map<std::type_index, System*> systems;

	// Set of entities that are going to be added or destroyed at the end of the frame.
	std::set<Entity> entitiesToDestroy;
	std::set<Entity> entitiesToCreate;

	
	
public:

	Manager() = default;

	void Update();

	Entity CreateEntity();
	
	void AddComponent<T>(Entity entity, T component);

	void AddEntityToSystem(Entity entity, System* system);
	

	
};
