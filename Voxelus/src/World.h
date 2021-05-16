#pragma once

#include "ECS/entities/Voxel.h"

class Ray;

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
	//		Update
	//

	void ProcessHoveringVoxels(const Ray& ray);
	void OnProcessTick();

	//-----------------------------------------------
	//		Getters
	//

	std::vector<std::shared_ptr<Voxel>>& GetVoxels();
	std::vector<std::shared_ptr<Voxel>>& GetCoordinateDirections();

	std::shared_ptr<Voxel> GetVoxelThatIntersectingWithPoint(glm::vec3 position);

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
	void GenerateCoordinateDirections();

	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	std::vector<std::shared_ptr<Voxel>> mVoxels;
	//std::vector<std::shared_ptr<Voxel>> mHoveredVoxels;
	std::shared_ptr<Voxel> mHoveredVoxel;
	//std::map<unsigned int, std::shared_ptr<Voxel>> mSelectedVoxelsIds;

	std::vector<std::shared_ptr<Voxel>> mCoordDirections;

	unsigned int mVoxelsIds[InitConstants::World::MaxSizeX][InitConstants::World::MaxSizeY][InitConstants::World::MaxSizeZ] = { 0 };

	std::map<unsigned int, std::shared_ptr<Voxel>> mVoxelsByIndexMap;

	unsigned int mWorldSizeX;
	unsigned int mWorldSizeY;
	unsigned int mWorldSizeZ;
};