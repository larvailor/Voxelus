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
	void OnProcessTick(bool* keys);

	//-----------------------------------------------
	//		Getters
	//

	std::vector<std::shared_ptr<Voxel>>& GetVoxels();
	std::vector<std::shared_ptr<Voxel>>& GetPossibleVoxels();
	std::vector<std::shared_ptr<Voxel>>& GetCoordinateDirections();

	std::shared_ptr<Voxel> GetVoxelThatIntersectingWithPoint(glm::vec3 position);

	//-----------------------------------------------
	//		Else
	//

	void CreateVoxel(const glm::vec3 position, const glm::vec4 color);
	void Clear();

private:
	/////////////////////////////////////////////////
	// 
	//		Methods
	//
	/////////////////////////////////////////////////
	
	//-----------------------------------------------
	//		Else
	//

	std::shared_ptr<Voxel> SpawnVoxel(glm::vec3 position, std::vector<std::shared_ptr<Voxel>>& voxels, bool isPossibleNewVoxel = false);
	glm::vec3 GetNewVoxelPosition();
	glm::uvec3 GetNewVoxelWorldIndex(const glm::vec3& position);
	void GenerateFloor();
	void GenerateCoordinateDirections();

	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	std::vector<std::shared_ptr<Voxel>> mVoxels;

	bool isSpawningInProgress = false;
	glm::vec3 mFirstPossibleVoxelCoords;
	glm::vec3 mLastPossibleVoxelCoords;
	std::vector<std::shared_ptr<Voxel>> mPossibleVoxels;

	std::shared_ptr<Voxel> mHoveredVoxel;

	//std::map<unsigned int, std::shared_ptr<Voxel>> mSelectedVoxelsIds;

	std::vector<std::shared_ptr<Voxel>> mCoordDirections;

	unsigned int mVoxelsIds[InitConstants::World::MaxSizeX][InitConstants::World::MaxSizeY][InitConstants::World::MaxSizeZ] = { 0 };

	std::map<unsigned int, std::shared_ptr<Voxel>> mVoxelsByIndexMap;

	unsigned int mWorldSizeX;
	unsigned int mWorldSizeY;
	unsigned int mWorldSizeZ;
};