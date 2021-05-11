#include "stdfax.h"

#include "World.h"

#include "ECS/components/Components.h"
#include "Ray.h"

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

World::World(unsigned int maxSizeX, unsigned int maxSizeY, unsigned int maxSizeZ)
	: mWorldSizeX(maxSizeX)
	, mWorldSizeY(maxSizeY)
	, mWorldSizeZ(maxSizeZ)
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
//		Update
//

void World::ProcessHoveringVoxels(const Ray& ray)
{
	for (std::shared_ptr<Voxel>& hoveredVoxel : mHoveredVoxels)
	{
		hoveredVoxel->SetHovered(false);
	}
	mHoveredVoxels.clear();

	float maxRayLenght = 1000.0f;
	glm::vec3 currRayPosition;
	for (float length = 1.0f; length < maxRayLenght; length += 1.0f)
	{
		currRayPosition = ray.GetOrigin() + ray.GetDirection() * length;

		std::shared_ptr<Voxel> voxel = GetVoxelThatIntersectingWithPoint(currRayPosition);
		if (voxel != nullptr)
		{
			voxel->SetHovered(true);
			mHoveredVoxels.push_back(voxel);
			break;
		}
	}
}

//-----------------------------------------------
//		Getters
//

std::vector<std::shared_ptr<Voxel>>& World::GetVoxels()
{
	return mVoxels;
}

std::vector<std::shared_ptr<Voxel>>& World::GetCoordinateDirections()
{
	return mCoordDirections;
}

std::shared_ptr<Voxel> World::GetVoxelThatIntersectingWithPoint(glm::vec3 position)
{
	int x = floor(position.x / InitConstants::Voxel::WidthX);
	int y = floor(position.y / InitConstants::Voxel::WidthY);
	int z = floor(position.z / InitConstants::Voxel::WidthZ);

	if (x >= 0 && x < mWorldSizeX &&
		y >= 0 && y < mWorldSizeY &&
		z >= 0 && z < mWorldSizeZ)
	{
		if (mVoxelsIds[x][y][z] != 0)
		{
			return mVoxelsByIndexMap[mVoxelsIds[x][y][z]];
		}
	}

	return nullptr;
}

/////////////////////////////////////////////////
// 
//		PRIVATE METHODS
//
/////////////////////////////////////////////////


void World::GenerateFloor()
{
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	float offsetZ = 0.0f;
	bool blackVoxel = true;
	for (unsigned int x = 0; x < mWorldSizeX; x++)
	{
		blackVoxel = x % 2 == 0;
		offsetX = x * InitConstants::Voxel::WidthX;
		for (unsigned int z = 0; z < mWorldSizeZ; z++)
		{
			std::shared_ptr<Voxel> floorVoxel = std::make_shared<Voxel>();
			floorVoxel->SetBaseColor(blackVoxel ? glm::vec4(0.7f, 0.7f, 0.7f, 1.0f) : glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
			blackVoxel = !blackVoxel;

			offsetZ = z * InitConstants::Voxel::WidthZ;
			floorVoxel->GetComponent<TransformComponent>()->SetPosition(glm::vec3(offsetX, offsetY, offsetZ));

			mVoxels.push_back(floorVoxel);
			mVoxelsByIndexMap[floorVoxel->GetId()] = floorVoxel;

			mVoxelsIds[x][0][z] = floorVoxel->GetId();
		}
	}
}

void World::GenerateCoordinateDirections()
{
	std::shared_ptr<Voxel> xDirection = std::make_shared<Voxel>();
	xDirection->GetComponent<TransformComponent>()->SetPosition(glm::vec3(-5.0f, 20.0f, -5.0f));
	xDirection->GetComponent<TransformComponent>()->SetScale(glm::vec3(50.0f, 0.5f, 0.5f));
	xDirection->SetColor(glm::vec4(0.85f, 0.20f, 0.12f, 1.0f));
	mCoordDirections.push_back(xDirection);

	std::shared_ptr<Voxel> yDirection = std::make_shared<Voxel>();
	yDirection->GetComponent<TransformComponent>()->SetPosition(glm::vec3(-5.0f, 20.0f, -5.0f));
	yDirection->GetComponent<TransformComponent>()->SetScale(glm::vec3(0.5f, 50.0f, 0.5f));
	yDirection->SetColor(glm::vec4(0.12f, 0.85f, 0.20f, 1.0f));
	mCoordDirections.push_back(yDirection);

	std::shared_ptr<Voxel> zDirection = std::make_shared<Voxel>();
	zDirection->GetComponent<TransformComponent>()->SetPosition(glm::vec3(-5.0f, 20.0f, -5.0f));
	zDirection->GetComponent<TransformComponent>()->SetScale(glm::vec3(0.5f, 0.5f, 50.0f));
	zDirection->SetColor(glm::vec4(0.12f, 0.20f, 0.85f, 1.0f));
	mCoordDirections.push_back(zDirection);
}
