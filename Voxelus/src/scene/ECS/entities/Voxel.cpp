#include "stdfax.h"

#include "Voxel.h"

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

Voxel::Voxel()
	: mColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
	, mBaseColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
	, mHoveredColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
	, mSize(glm::vec3(InitConstants::Voxel::WidthX, InitConstants::Voxel::WidthY, InitConstants::Voxel::WidthZ))
	, mIsHovered(false)
	, mIsSelected(false)
	, Entity()
{
}

//-----------------------------------------------
//		Destructors
//

Voxel::~Voxel()
{
}

//-----------------------------------------------
//		Getters
//

glm::vec4 Voxel::GetColor() const
{
	return mColor;
}

glm::vec3 Voxel::GetSize() const
{
	return mSize;
}

Voxel::HoveredSide Voxel::GetHoveredSide() const
{
	return mHoveredSide;
}

//-----------------------------------------------
//		Setters
//

void Voxel::SetColor(glm::vec4 color)
{
	mColor = color;
}

void Voxel::SetBaseColor(glm::vec4 color)
{
	mColor = color;
	mBaseColor = color;
}

void Voxel::SetSize(glm::vec3 size)
{
	mSize = size;
}

void Voxel::SetHovered(bool isHovered)
{
	mIsHovered = isHovered;
	if (!isHovered)
	{
		//mHoveredSide = HoveredSide::None;
	}

	SetColor(isHovered ? mHoveredColor : mBaseColor);
}

void Voxel::SetSelected(bool isSelected)
{
	mIsSelected = isSelected;
}

//-----------------------------------------------
//		Collisions
//

Voxel::HoveredSide Voxel::GetPlaneNumberThatIntersectsWithRay(const Ray& ray, const glm::vec3& currRayPosition)
{
	std::shared_ptr<TransformComponent> transform = GetComponent<TransformComponent>();
	glm::vec3 voxelOrigin = transform->GetPosition();
	float delta = 0.05f;
	
	// back
	if (currRayPosition.x >= voxelOrigin.x && currRayPosition.x <= voxelOrigin.x + mSize.x)
	{
		if (currRayPosition.y >= voxelOrigin.y && currRayPosition.y <= voxelOrigin.y + mSize.y)
		{
			if (currRayPosition.z >= voxelOrigin.z - delta && currRayPosition.z <= voxelOrigin.z + delta)
			{
				mHoveredSide = HoveredSide::Back;
				return mHoveredSide;
			}
		}
	}

	// front
	if (currRayPosition.x >= voxelOrigin.x && currRayPosition.x <= voxelOrigin.x + mSize.x)
	{
		if (currRayPosition.y >= voxelOrigin.y && currRayPosition.y <= voxelOrigin.y + mSize.y)
		{
			if (currRayPosition.z >= voxelOrigin.z + mSize.z - delta && currRayPosition.z <= voxelOrigin.z + mSize.z + delta)
			{
				//std::cout << "Front" << std::endl;
				mHoveredSide = HoveredSide::Front;
				return mHoveredSide;
			}
		}
	}

	// left
	if (currRayPosition.x >= voxelOrigin.x - delta && currRayPosition.x <= voxelOrigin.x + delta)
	{
		if (currRayPosition.y >= voxelOrigin.y && currRayPosition.y <= voxelOrigin.y + mSize.y)
		{
			if (currRayPosition.z >= voxelOrigin.z && currRayPosition.z <= voxelOrigin.z + mSize.z)
			{
				//std::cout << "Left" << std::endl;
				mHoveredSide = HoveredSide::Left;
				return mHoveredSide;
			}
		}
	}

	// right
	if (currRayPosition.x >= voxelOrigin.x + mSize.z - delta && currRayPosition.x <= voxelOrigin.x + mSize.z + delta)
	{
		if (currRayPosition.y >= voxelOrigin.y && currRayPosition.y <= voxelOrigin.y + mSize.y)
		{
			if (currRayPosition.z >= voxelOrigin.z && currRayPosition.z <= voxelOrigin.z + mSize.z)
			{
				//std::cout << "Right" << std::endl;
				mHoveredSide = HoveredSide::Right;
				return mHoveredSide;
			}
		}
	}

	// bottom
	if (currRayPosition.x >= voxelOrigin.x && currRayPosition.x <= voxelOrigin.x + mSize.z)
	{
		if (currRayPosition.y >= voxelOrigin.y - delta && currRayPosition.y <= voxelOrigin.y + delta)
		{
			if (currRayPosition.z >= voxelOrigin.z && currRayPosition.z <= voxelOrigin.z + mSize.z)
			{
				//std::cout << "Bottom" << std::endl;
				mHoveredSide = HoveredSide::Bottom;
				return mHoveredSide;
			}
		}
	}

	// top
	if (currRayPosition.x >= voxelOrigin.x && currRayPosition.x <= voxelOrigin.x + mSize.z)
	{
		if (currRayPosition.y >= voxelOrigin.y + mSize.y - delta && currRayPosition.y <= voxelOrigin.y + mSize.y + delta)
		{
			if (currRayPosition.z >= voxelOrigin.z && currRayPosition.z <= voxelOrigin.z + mSize.z)
			{
				//std::cout << "Top" << std::endl;
				mHoveredSide = HoveredSide::Top;
				return mHoveredSide;
			}
		}
	}
}

/////////////////////////////////////////////////
// 
//		PRIVATE METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Collisions
//

bool Voxel::RayPlaneIntersection(const glm::vec3& planeNormal, const glm::vec3& planePoint, const Ray& ray)
{
	glm::vec3 smallRay = ray.GetOrigin() + ray.GetDirection() * ray.GetLength() - ray.GetOrigin();
	glm::vec3 rayOrigin_PlanePoint = planePoint - ray.GetOrigin();

	float k = (rayOrigin_PlanePoint.x * planeNormal.x + rayOrigin_PlanePoint.y * planeNormal.y + rayOrigin_PlanePoint.z * planeNormal.z) / (smallRay.x * planeNormal.x + smallRay.y * planeNormal.y + smallRay.z * planeNormal.z);
	
	glm::vec3 intersectionPoint = ray.GetOrigin() + k * smallRay;

	return k >= 0 && k <= 1;
}
