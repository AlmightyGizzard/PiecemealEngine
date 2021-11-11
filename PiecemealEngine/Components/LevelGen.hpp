#pragma once
#include "Core/Conductor.hpp"
#include "Components/ComponentTest.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>


class LevelGen 
{
public:
	int noRooms;
	int gridSize;
	std::vector<Entity> levelEntities;

	void Init(int rooms, int grid = 50);

	// Currently generates from a CSV - at some point implement a basic procedural system
	// and output the level into CSV
	void erate(std::string myFile);
	std::vector<Entity> GetLevelEntities();

	
};