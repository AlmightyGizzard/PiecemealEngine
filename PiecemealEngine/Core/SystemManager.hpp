#pragma once

#include "System.hpp"
#include "Types.hpp"
#include <cassert>
#include <memory>
#include <unordered_map>

// The system manager works similarly to the component manager:
// It takes a record of registered systems and their signatures.

// Each registered system is added to a map with the same typeid.name technique
// used in the component manager. 

// Similarly, this does mean that RegisterSystem will need to be called for every new system added to the game.

class SystemManager {
public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem() {
		const char* typeName = typeid(T).name();

		assert(systems.find(typeName) == systems.end() && "Registering a system more than once");

		// Create a pointer to the system and return it for use.
		auto system = std::make_shared<T>();
		systems.insert({ typeName, system });
		return system;
	}

	template<typename T>
	void SetSignature(Signature signature) {
		const char* typeName = typeid(T).name();

		assert(systems.find(typeName) != systems.end() && "System used before registered");

		signatures.insert({ typeName, signature });		
	}

	void EntityDestroyed(Entity entity) {
		// Erase a destroyed entity from all system lists, so it isn't still beholden
		// to gravityy after death etc
		for (auto const& pair : systems) {
			auto const& system = pair.second;
			system->involvedEntities.erase(entity);
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature) {
		for (auto const& pair : systems) {
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = signatures[type];

			// If entity sig matches system sig, insert it into the set
			if ((entitySignature & systemSignature) == systemSignature) {
				system->involvedEntities.insert(entity);
			}
			// else bin it
			else {
				system->involvedEntities.erase(entity);
			}
		}
	}
	// lmao
private:
	// Oh hey, two maps again

	std::unordered_map<const char*, Signature> signatures{};

	std::unordered_map<const char*, std::shared_ptr<System>> systems{};
};