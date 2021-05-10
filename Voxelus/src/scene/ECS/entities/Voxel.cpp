#include "stdfax.h"

#include "Voxel.h"


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
	, mSize(glm::vec3(InitConstants::Voxel::WidthX, InitConstants::Voxel::WidthY, InitConstants::Voxel::WidthZ))
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

void Voxel::SetSize(glm::vec3 size)
{
	mSize = size;
}
