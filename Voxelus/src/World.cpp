#include "stdfax.h"

#include "World.h"

#include "ECS/components/Components.h"

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

World::World(unsigned int maxSizeX, unsigned int maxSizeY, unsigned int maxSizeZ)
	: mSizeX(maxSizeX)
	, mSizeY(maxSizeY)
	, mSizeZ(maxSizeZ)
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
	float offsetX = 0.0f;
	float offsetY = -InitConstants::Voxel::WidthY;
	float offsetZ = 0.0f;
	bool blackVoxel = true;
	for (unsigned int x = 0; x < mSizeX; x++)
	{
		blackVoxel = x % 2 == 0;
		offsetX = x * InitConstants::Voxel::WidthX;
		for (unsigned int z = 0; z < mSizeZ; z++)
		{
			std::shared_ptr<Voxel> floorVoxel = std::make_shared<Voxel>();
			floorVoxel->SetColor(blackVoxel ? glm::vec4(0.7f, 0.7f, 0.7f, 1.0f) : glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
			blackVoxel = !blackVoxel;

			offsetZ = z * InitConstants::Voxel::WidthZ;
			floorVoxel->GetComponent<TransformComponent>()->SetPosition(glm::vec3(offsetX, offsetY, offsetZ));

			mFloor.push_back(floorVoxel);
		}
	}
}
