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
	GenerateCoordinateDirections();
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

std::vector<std::shared_ptr<Voxel>>& World::GetCoordinateDirections()
{
	return mCoordDirections;
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

void World::GenerateCoordinateDirections()
{
	std::shared_ptr<Voxel> xDirection = std::make_shared<Voxel>();
	xDirection->GetComponent<TransformComponent>()->SetPosition(glm::vec3(-5.0f, 0.0f, -5.0f));
	xDirection->GetComponent<TransformComponent>()->SetScale(glm::vec3(50.0f, 0.5f, 0.5f));
	xDirection->SetColor(glm::vec4(0.85f, 0.20f, 0.12f, 1.0f));
	mCoordDirections.push_back(xDirection);

	std::shared_ptr<Voxel> yDirection = std::make_shared<Voxel>();
	yDirection->GetComponent<TransformComponent>()->SetPosition(glm::vec3(-5.0f, 0.0f, -5.0f));
	yDirection->GetComponent<TransformComponent>()->SetScale(glm::vec3(0.5f, 50.0f, 0.5f));
	//yDirection->SetSize(glm::vec3(5.0f, 150.0f, 5.0f));
	yDirection->SetColor(glm::vec4(0.12f, 0.85f, 0.20f, 1.0f));
	mCoordDirections.push_back(yDirection);

	std::shared_ptr<Voxel> zDirection = std::make_shared<Voxel>();
	zDirection->GetComponent<TransformComponent>()->SetPosition(glm::vec3(-5.0f, 0.0f, -5.0f));
	zDirection->GetComponent<TransformComponent>()->SetScale(glm::vec3(0.5f, 0.5f, 50.0f));
	//zDirection->SetSize(glm::vec3(5.0f, 5.0f, 150.0f));
	zDirection->SetColor(glm::vec4(0.12f, 0.20f, 0.85f, 1.0f));
	mCoordDirections.push_back(zDirection);
}
