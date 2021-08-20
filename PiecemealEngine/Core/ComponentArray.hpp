#pragma once

#include "Types.hpp"
#include <array>
#include <cassert>
#include <unordered_map>


// There is an instance of inheritance at the beginning here, this
// impacts performance but allows the COmponentManager to tell a 
// generic componentArray that an entity has been destroyed, 
// prompting an update to it's mapping.

// Every component will have a corresponding componentArray attached to it, 
// that will store every entity that currently has that component attached.
// Furthermore, it has functions that allow us to add/remove entity IDs whenever
// we create new entities that have said components.

// This way, instead of looping through every entity with a physics component, 
// we can simply loop through every physics component, all stored in the same spot
// in memory.

class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
public:
	void InsertData(Entity entity, T component) 
	{
		assert(entityToIndexMap.find(entity) == entityToIndexMap.end() && "Component added to same entity more than once.");
				
		// Put a new entry to the array at the end and update both maps
		size_t newIndex = currentArraySize;
		entityToIndexMap[entity] = newIndex;
		indexToEntityMap[newIndex] = entity;
		componentArray[newIndex] = component;
		currentArraySize++;
	}

	void RemoveData(Entity entity) 
	{
		assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Removing non-existent component.");

		// Copy the element at the back into the deleted elements place to keep it dense, yo
		size_t indexOfRemoved = entityToIndexMap[entity];
		size_t indexOfLast = currentArraySize - 1;
		componentArray[indexOfRemoved] = componentArray[indexOfLast];

		// Update the maps so that they point to the correct spot
		Entity entityOfLastElement = entityToIndexMap[entity];
		entityToIndexMap[entityOfLastElement] = indexOfRemoved;
		indexToEntityMap[indexOfRemoved] = entityOfLastElement;

		// erase the entity at the back
		entityToIndexMap.erase(entity);
		indexToEntityMap.erase(indexOfLast);

		currentArraySize--;
	}

	T& GetData(Entity entity) 
	{
		assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Retrieving non-existent component.");
		
		// return a reference to the entities component
		return componentArray[entityToIndexMap[entity]];
	}

	void EntityDestroyed(Entity entity) override
	{
		if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
			// remove the entities component(s) if it existed
			RemoveData(entity);
		}
	}

private:
	// The packed array we are trying to create - with a generic type T
	// it is set to a specified max amount == MAX_ENTITIES, so that even
	// if we reach that value each entity will occupy its own spot in the array.
	std::array<T, MAX_ENTITIES> componentArray;

	// We also create an unordered map from the entity ID to an array index...
	std::unordered_map<Entity, size_t> entityToIndexMap;

	//...and create the reverse, a map from an array index to an entity ID.
	std::unordered_map<size_t, Entity> indexToEntityMap;

	// size of valid entries in the array
	size_t currentArraySize;
};