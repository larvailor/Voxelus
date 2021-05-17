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
	if (mHoveredVoxel)
	{
		mHoveredVoxel->SetHovered(false);
	}
	mHoveredVoxel = nullptr;

	float maxRayLenght = 1000.0f;
	glm::vec3 currRayPosition;
	for (float length = 1.0f; length < maxRayLenght; length += 0.2f)
	{
		currRayPosition = ray.GetOrigin() + ray.GetDirection() * length;

		std::shared_ptr<Voxel> voxel = GetVoxelThatIntersectingWithPoint(currRayPosition);
		if (voxel != nullptr)
		{
			voxel->SetHovered(true);
			voxel->GetPlaneNumberThatIntersectsWithRay(ray, currRayPosition);
			mHoveredVoxel = voxel;
			break;
		}
	}
}

void World::OnProcessTick(bool* keys)
{
	// spawn voxel
	if (Mouse::mWasLMB_Pressed && Instruments::Current == Instruments::Type::Add)
	{
		if (mHoveredVoxel)
		{
			glm::vec3 newVoxelPosition = GetNewVoxelPosition();
			std::shared_ptr<Voxel> newVoxel = SpawnVoxel(newVoxelPosition, mVoxels);
		}
	}

	// shift voxel spawn
	if (keys[GLFW_KEY_LEFT_SHIFT] && Instruments::Current == Instruments::Type::Add)
	{
		if (Mouse::mWasLMB_Pressed && !isSpawningInProgress)
		{
			if (mHoveredVoxel)
			{
				mFirstPossibleVoxelCoords = GetNewVoxelPosition();
				isSpawningInProgress = true;
			}
		}
		if (Mouse::mIsLMB_Pressed && !Mouse::mWasLMB_Released && isSpawningInProgress)
		{
			if (mHoveredVoxel)
			{
				mLastPossibleVoxelCoords = GetNewVoxelPosition();
			}

			mPossibleVoxels.clear();

			glm::uvec3 firstPossibleVoxelCoords = GetNewVoxelWorldIndex(mFirstPossibleVoxelCoords);
			glm::uvec3 lastPossibleVoxelCoords = GetNewVoxelWorldIndex(mLastPossibleVoxelCoords);

			unsigned int xStart;
			unsigned int xEnd;

			if (static_cast<int>(lastPossibleVoxelCoords.x - firstPossibleVoxelCoords.x) >= 0)
			{
				xStart = firstPossibleVoxelCoords.x;
				xEnd = lastPossibleVoxelCoords.x;
			}
			else
			{
				xStart = lastPossibleVoxelCoords.x;
				xEnd = firstPossibleVoxelCoords.x;
			}
			if (xStart < 0)
			{
				xStart = 0;
			}
			if (xEnd > mWorldSizeX)
			{
				xEnd = mWorldSizeX - 1;
			}

			unsigned int yStart;
			unsigned int yEnd;
			if (static_cast<int>(lastPossibleVoxelCoords.y - firstPossibleVoxelCoords.y) >= 0)
			{
				yStart = firstPossibleVoxelCoords.y;
				yEnd = lastPossibleVoxelCoords.y;
			}
			else
			{
				yStart = lastPossibleVoxelCoords.y;
				yEnd = firstPossibleVoxelCoords.y;
			}
			if (yStart < 0)
			{
				yStart = 0;
			}
			if (yEnd > mWorldSizeY)
			{
				yEnd = mWorldSizeY - 1;
			}

			unsigned int zStart;
			unsigned int zEnd;
			if (static_cast<int>(lastPossibleVoxelCoords.z - firstPossibleVoxelCoords.z) >= 0)
			{
				zStart = firstPossibleVoxelCoords.z;
				zEnd = lastPossibleVoxelCoords.z;
			}
			else
			{
				zStart = lastPossibleVoxelCoords.z;
				zEnd = firstPossibleVoxelCoords.z;
			}
			if (zStart < 0)
			{
				zStart = 0;
			}
			if (zEnd > mWorldSizeZ)
			{
				zEnd = mWorldSizeZ - 1;
			}


			for (unsigned int y = yStart; y <= yEnd; y++)
			{
				for (unsigned int x = xStart; x <= xEnd; x++)
				{
					for (unsigned int z = zStart; z <= zEnd; z++)
					{
						glm::vec3 position = glm::vec3(x * 20.0f, y * 20.0f, z * 20.0f);
						SpawnVoxel(position, mPossibleVoxels, true);
					}
				}
			}

			//std::cout << xStart << " " << xEnd << std::endl;
		}
		if (Mouse::mWasLMB_Released && isSpawningInProgress)
		{
			isSpawningInProgress = false;

			for (std::shared_ptr<Voxel>& possibleVoxel : mPossibleVoxels)
			{
				mVoxelsByIndexMap[possibleVoxel->GetId()] = possibleVoxel;
				glm::vec3 possiblePosition = possibleVoxel->GetComponent<TransformComponent>()->GetPosition();
				glm::uvec3 possibleWorldIndex = GetNewVoxelWorldIndex(possiblePosition);
				mVoxelsIds[possibleWorldIndex.x][possibleWorldIndex.y][possibleWorldIndex.z] = possibleVoxel->GetId();
				mVoxels.push_back(possibleVoxel);
			}

			mPossibleVoxels.clear();
		}
	}
	else
	if (!keys[GLFW_KEY_LEFT_SHIFT] && isSpawningInProgress)
	{
		isSpawningInProgress = false;

		for (std::shared_ptr<Voxel>& possibleVoxel : mPossibleVoxels)
		{
			mVoxelsByIndexMap[possibleVoxel->GetId()] = possibleVoxel;
			glm::vec3 possiblePosition = possibleVoxel->GetComponent<TransformComponent>()->GetPosition();
			glm::uvec3 possibleWorldIndex = GetNewVoxelWorldIndex(possiblePosition);
			mVoxelsIds[possibleWorldIndex.x][possibleWorldIndex.y][possibleWorldIndex.z] = possibleVoxel->GetId();
			mVoxels.push_back(possibleVoxel);
		}

		mPossibleVoxels.clear();
	}

	// Delete
	if (Instruments::Current == Instruments::Type::Delete && Mouse::mWasLMB_Pressed)
	{
		if (mHoveredVoxel)
		{
			glm::vec3 hoveredVoxelPosition = mHoveredVoxel->GetComponent<TransformComponent>()->GetPosition();
			glm::uvec3 mHoveredVoxelWorldIndex = GetNewVoxelWorldIndex(hoveredVoxelPosition);
			mVoxelsIds[mHoveredVoxelWorldIndex.x][mHoveredVoxelWorldIndex.y][mHoveredVoxelWorldIndex.z] = 0;
			mVoxelsByIndexMap.erase(mVoxelsByIndexMap.find(mHoveredVoxel->GetId()));

			int hoveredIndex = -1;
			for (unsigned int index = 0; index < mVoxels.size(); index++)
			{
				if (mVoxels[index]->GetId() == mHoveredVoxel->GetId())
				{
					hoveredIndex = index;
					break;
				}
			}

			if (hoveredIndex != -1)
			{
				mVoxels.erase(mVoxels.begin() + hoveredIndex);
			}
		}
	}
	
	// Brush
	if (Instruments::Current == Instruments::Type::Brush && Mouse::mIsLMB_Pressed)
	{
		if (mHoveredVoxel)
		{
			mHoveredVoxel->SetBaseColor(glm::vec4(Instruments::BrushColorR, Instruments::BrushColorG, Instruments::BrushColorB, 1.0f));
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

std::vector<std::shared_ptr<Voxel>>& World::GetPossibleVoxels()
{
	return mPossibleVoxels;
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


std::shared_ptr<Voxel> World::SpawnVoxel(const glm::vec3 position, std::vector<std::shared_ptr<Voxel>>& voxels, bool isPossibleNewVoxel)
{
	glm::uvec3 newVoxelWorldIndex = GetNewVoxelWorldIndex(position);

	if (newVoxelWorldIndex.x != -1.0f &&
		newVoxelWorldIndex.y != -1.0f &&
		newVoxelWorldIndex.z != -1.0f)
	{
		if (mVoxelsIds[newVoxelWorldIndex.x][newVoxelWorldIndex.y][newVoxelWorldIndex.z] == 0)
		{
			std::shared_ptr<Voxel> newVoxel = std::make_shared<Voxel>();
			newVoxel->GetComponent<TransformComponent>()->SetPosition(position);
			newVoxel->SetBaseColor(glm::vec4(Instruments::BrushColorR, Instruments::BrushColorG, Instruments::BrushColorB, 1.0f));

			if (!isPossibleNewVoxel)
			{
				mVoxelsByIndexMap[newVoxel->GetId()] = newVoxel;
				mVoxelsIds[newVoxelWorldIndex.x][newVoxelWorldIndex.y][newVoxelWorldIndex.z] = newVoxel->GetId();
			}
			voxels.push_back(newVoxel);

			return newVoxel;
		}
	}

	return nullptr;
}

glm::vec3 World::GetNewVoxelPosition()
{
	glm::vec3 hoveredVoxelPos = mHoveredVoxel->GetComponent<TransformComponent>()->GetPosition();
	glm::vec3 newVoxelPos = hoveredVoxelPos;
	switch (mHoveredVoxel->GetHoveredSide())
	{
	case Voxel::HoveredSide::Back:
		newVoxelPos.z = newVoxelPos.z - mHoveredVoxel->GetSize().z;
		break;
	case Voxel::HoveredSide::Front:
		newVoxelPos.z = newVoxelPos.z + mHoveredVoxel->GetSize().z;
		break;
	case Voxel::HoveredSide::Left:
		newVoxelPos.x = newVoxelPos.x - mHoveredVoxel->GetSize().x;
		break;
	case Voxel::HoveredSide::Right:
		newVoxelPos.x = newVoxelPos.x + mHoveredVoxel->GetSize().x;
		break;
	case Voxel::HoveredSide::Bottom:
		newVoxelPos.y = newVoxelPos.y - mHoveredVoxel->GetSize().y;
		break;
	case Voxel::HoveredSide::Top:
		newVoxelPos.y = newVoxelPos.y + mHoveredVoxel->GetSize().y;
		break;

	case Voxel::HoveredSide::None:
		break;
	}
	return newVoxelPos;
}

glm::uvec3 World::GetNewVoxelWorldIndex(const glm::vec3& position)
{
	unsigned int x = static_cast<unsigned int>(position.x / 20.0f);
	unsigned int y = static_cast<unsigned int>(position.y / 20.0f);
	unsigned int z = static_cast<unsigned int>(position.z / 20.0f);
	if (x >= 0 && x < mWorldSizeX &&
		y >= 0 && y < mWorldSizeY &&
		z >= 0 && z < mWorldSizeZ)
	{
		return glm::uvec3(x, y, z);
	}

	return glm::uvec3(-1, -1, -1);
}

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
