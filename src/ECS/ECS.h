#pragma once

#include <bitset>
#include <vector>
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

	std::vector<Entity> GetEntities() const;
	Signature& GetComponentSignature() const;
};

class Component {

};

class Manager {

};
