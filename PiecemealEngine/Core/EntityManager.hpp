#include "Types.hpp"
#include <array>
#include <cassert>
#include <queue>


class EntityManager {
public:
	EntityManager() {

		for (Entity entity = 0; entity < MAX_ENTITIES; entity++) {
			availableEntityIDs.push(entity);
			
		}
	}

	Entity CreateEntity() {
		assert(livingEntities < MAX_ENTITIES && "Too many Entities");

		// Take an ID from the front, and use it :P
		Entity id = availableEntityIDs.front();
		availableEntityIDs.pop();
		livingEntities++;

		return id;
	}

	void DestroyEntity(Entity entity) {
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		// remove any data by resetting the entity
		attachedSignatures[entity].reset();

		// Chuck it to the back with the rest
		availableEntityIDs.push(entity);
		livingEntities--;
	}

	void SetSignature(Entity entity, Signature signature) {
		assert(entity < MAX_ENTITIES && "Entity out of range");

		// put this entities signature into the array. 
		attachedSignatures[entity] = signature;
		
	}

	Signature GetSignature(Entity entity) {
		assert(entity < MAX_ENTITIES && "Entity out of range");

		//return the signature from the array
		return attachedSignatures[entity];
	}
private:
	// A queue of unused entity IDs
	std::queue<Entity> availableEntityIDs{};

	// An array of signatures (0b0101 etc) where the index matches
	// the entity ID
	std::array<Signature, MAX_ENTITIES> attachedSignatures{};

	uint32_t livingEntities{};
};