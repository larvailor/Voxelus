#include "stdfax.h"

#include "World.h"

#include "ECS/components/Components.h"

namespace
{
	unsigned int SizeX = 32;
	unsigned int SizeY = 16;
	unsigned int SizeZ = 32;

	glm::vec3 DarkGrey = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 LightGrey = glm::vec3(0.8f, 0.8f, 0.8f);
}

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

World::World()
	: mSizeX(SizeX)
	, mSizeY(SizeY)
	, mSizeZ(SizeZ)
{
	GenerateFloor();
}

//-----------------------------------------------
//		Destructors
//

World::~World()
{
}

//-----------------------------------------------
//		Getters
//

std::vector<std::shared_ptr<Voxel>>& World::GetVoxels()
{
	return mVoxels;
}

std::vector<std::shared_ptr<Voxel>>& World::GetFloor()
{
	return mFloor;
}

/////////////////////////////////////////////////
// 
//		PRIVATE METHODS
//
/////////////////////////////////////////////////


void World::GenerateFloor()
{
	float voxelHalfWidth = 10.0f;
	float voxelWidth = 2 * voxelHalfWidth;
	float offsetX = 0.0f;
	float offsetY = -voxelHalfWidth;
	float offsetZ = 0.0f;
	bool blackVoxel = true;
	for (unsigned int x = 0; x < mSizeX; x++)
	{
		blackVoxel = x % 2 == 0;
		offsetX = x * voxelWidth + voxelHalfWidth;
		for (unsigned int z = 0; z < mSizeZ; z++)
		{
			std::shared_ptr<Voxel> floorVoxel = std::make_shared<Voxel>();
			floorVoxel->SetColor(blackVoxel ? glm::vec3(0.2f, 0.2f, 0.2f) : glm::vec3(0.9f, 0.9f, 0.9f));
			blackVoxel = !blackVoxel;

			offsetZ = z * voxelWidth + voxelHalfWidth;
			floorVoxel->GetComponent<TransformComponent>()->SetPosition(glm::vec3(offsetX, offsetY, offsetZ));

			mFloor.push_back(floorVoxel);
		}
	}
}
