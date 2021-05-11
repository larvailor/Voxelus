#include "stdfax.h"

#include "Voxel.h"

#include "ECS/components/Components.h"

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
	, mHoveredColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))
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
	SetColor(isHovered ? mHoveredColor : mBaseColor);
}

void Voxel::SetSelected(bool isSelected)
{
	mIsSelected = isSelected;
}

//-----------------------------------------------
//		Collisions
//

bool Voxel::Intersects(glm::vec3 position)
{
	glm::vec3 minPos = GetComponent<TransformComponent>()->GetPosition();
	return false;
}
