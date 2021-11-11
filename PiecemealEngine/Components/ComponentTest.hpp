#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Gravity {
	glm::vec3 force;
};

struct RigidBody {
	glm::vec3 velocity;
	glm::vec3 acceleration;
};

struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

struct Renderable
{
	glm::vec3 color;
};


// Player input (component): series of boolean values for keypresses

// Player Controller (system) takes an input component and applies forces to the 
// given rigidbody, need to also work out mouse input and connect it to this system

// vision cones (renderer) 