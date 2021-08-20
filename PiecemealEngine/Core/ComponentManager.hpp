#pragma once

#include "ComponentArray.hpp"
#include "Types.hpp"
#include <any>
#include <memory>
#include <unordered_map>

// The manager talks to every componentArray whenever 
// components are added or removed.

class ComponentManager
{
public:
	template<typename T>
	void RegisterComponent() 
	{
		const char* typeName = typeid(T).name();

		assert(componentTypes.find(typeName) == componentTypes.end() && "Registering Component Type more than once");

		// Add this component type to the array
		componentTypes.insert({ typeName, nextComponentType });

		// Create a componentArray pointer and add it to the component array map
		componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

		nextComponentType++;
	}

	template<typename T>
	ComponentType GetComponentType() 
	{
		const char* typeName = typeid(T).name();

		assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered");

		//return this component's type - used for creating a signature
		return componentTypes[typeName];
	}

	template<typename T>
	void AddComponent(Entity entity, T component) {
		// Add a component to the array for an entity
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity, T component) {
		// Remove a component from the array
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(Entity entity) {
		// get a ref to a component from the array
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(Entity entity) {
		// Notify every component array than an entity has been destroyed, 
		// if the array has a matching entity registered, it will remove it.
		for (auto const& pair : componentArrays) {
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
		}
	}

private:
	// We need a unique ID for every type of component - componentType will increment
	// by one for every new componentType created. This means non-base components will
	// need to be registered, but it'll do.

	// C++ has a way of returnng a pointer to a const char array of type T - since that 
	// pointer is just an integer, we can use it as a key in a map of ComponentTypes,
	// AS WELL AS IComponentArray pointers, ensuring there is only a single ComponentArray
	// instantiation per ComponentType. Poggers.

	// Map from type string pointer to a component type
	std::unordered_map<const char*, ComponentType> componentTypes{};

	// Map from a type string pointer to a component Array
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays{};

	// the component type to be assigned to the next new component
	ComponentType nextComponentType{};

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray() {
		const char* typeName = typeid(T).name();

		assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
	}
};
