#pragma once

#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "SystemManager.hpp"
#include "SDL.h"
#include "Types.hpp"
#include <memory>
#include <iostream>


class Conductor {
public:
	void Init() {
		entityManager = std::make_unique<EntityManager>();
		componentManager = std::make_unique<ComponentManager>();
		systemManager = std::make_unique<SystemManager>();
	}

	// ENTITY
	Entity CreateEntity() {
		return entityManager->CreateEntity();
	}

	void DestroyEntity(Entity entity) {
		// When you destroy an entity, you need to remove it from all the separate arrays
		// it is keyed into. thus, this function interacts with all managers.
		entityManager->DestroyEntity(entity);
		componentManager->EntityDestroyed(entity);
		systemManager->EntityDestroyed(entity);
	}

	template<typename T>
	bool HasComponent(Entity entity) {
		
		auto signature = entityManager->GetSignature(entity);
		return signature.test(componentManager->GetComponentType<T>());
	}

	// COMPONENT
	template<typename T>
	void RegisterComponent() {
		componentManager->RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(Entity entity, T component) {
		componentManager->AddComponent<T>(entity, component);

		auto signature = entityManager->GetSignature(entity);
		// set the corresponding component bit in the binary signature to 1. 
		signature.set(componentManager->GetComponentType<T>(), true);
		// Now that the entity has a new component, its binary signature needs to be updated,
		// hence, we need to update the Systems in case it no longer qualifies/suddenly
		// qualifies for certain systems.

		// This bit isn't happening I don't think.
		entityManager->SetSignature(entity, signature);
		systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void RemoveComponent(Entity entity) {
		componentManager->RemoveComponent<T>(entity);

		auto signature = entityManager->GetSignature(entity);
		// set the corresponding component bit in the binary signature to 0.
		signature.set(componentManager->GetComponentType<T>(), false);

		entityManager->SetSignature(entity, signature);
		systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& GetComponent(Entity entity) {
		return componentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType GetComponentType() {
		return componentManager->GetComponentType<T>();
	}

	// SYSTEM

	template<typename T>
	std::shared_ptr<T> RegisterSystem() {
		return systemManager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature) {
		systemManager->SetSignature<T>(signature);
	}

	Signature GetEntitySignature(Entity entity) {
		return entityManager->GetSignature(entity);
	}
private:
	// All coming together isn't it?
	std::unique_ptr<EntityManager> entityManager;
	std::unique_ptr<ComponentManager> componentManager;
	std::unique_ptr<SystemManager> systemManager;
};