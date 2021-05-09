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
	: mColor(glm::vec3(1.0f, 1.0f, 1.0f))
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

glm::vec3 Voxel::GetColor() const
{
	return mColor;
}

//-----------------------------------------------
//		Setters
//

void Voxel::SetColor(glm::vec3 color)
{
	mColor = color;
}
