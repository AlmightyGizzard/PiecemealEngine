#include "PhysicsSystem.hpp"

#include "Components/ComponentTest.hpp"
#include "Core/Conductor.hpp"

extern Conductor conductor;

void PhysicsSystem::Init() {

}

void PhysicsSystem::Update(float dt) {
	for (auto const& entity : involvedEntities) {
		auto& rigidbody = conductor.GetComponent<RigidBody>(entity);
		auto& transform = conductor.GetComponent<Transform>(entity);

		// Forces
		auto const& gravity = conductor.GetComponent<Gravity>(entity);

		transform.position += rigidbody.velocity * dt;
		rigidbody.velocity += gravity.force * dt;
	}
}