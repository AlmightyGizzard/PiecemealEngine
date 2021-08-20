#pragma once

#include <cstdint>
#include <bitset>

// ECS system using types + signatures


// Gives Entities an ID, and sets a maximum number of entities.
// An Entity is nothing more than this ID.
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;

// Similarly, we define a typeid for components, as well as determining
// the max size of arrays of components.
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

// This creates a bitset from which we can assign an entity components. 
// If transform has type 0, rigidbody has type 1, and gravity has type 2,
// then a floating object with collisions would read as 0b110
using Signature = std::bitset<MAX_COMPONENTS>;

// This system above was a massive help in terms of kickstarting this
// process, and is thanks to Austin Morlan

// https://austinmorlan.com/posts/entity_component_system/#source-code
