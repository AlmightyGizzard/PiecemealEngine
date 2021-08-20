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