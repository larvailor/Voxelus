#pragma once

#include "ECS/entities/Voxel.h"

class World
{
public:
	/////////////////////////////////////////////////
	// 
	//		Methods
	//
	/////////////////////////////////////////////////

	//-----------------------------------------------
	//		Constructors
	//

	World(unsigned int maxSizeX, unsigned int maxSizeY, unsigned int maxSizeZ);

	//-----------------------------------------------
	//		Destructors
	//

	~World();

	//-----------------------------------------------
	//		Getters
	//

	std::vector<std::shared_ptr<Voxel>>& GetVoxels();
	std::vector<std::shared_ptr<Voxel>>& GetFloor();

private:
	/////////////////////////////////////////////////
	// 
	//		Methods
	//
	/////////////////////////////////////////////////
	
	//-----------------------------------------------
	//		Else
	//

	void GenerateFloor();

	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	std::vector<std::shared_ptr<Voxel>> mVoxels;
	std::vector<std::shared_ptr<Voxel>> mFloor;

	unsigned int mSizeX;
	unsigned int mSizeY;
	unsigned int mSizeZ;
};