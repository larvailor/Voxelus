#pragma once

#include "Entity.h"

class Voxel : public Entity
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

	Voxel();

	//-----------------------------------------------
	//		Destructors
	//

	~Voxel();

	//-----------------------------------------------
	//		Getters
	//
	
	glm::vec4 GetColor() const;
	glm::vec3 GetSize() const;
	
	//-----------------------------------------------
	//		Setters
	//

	void SetColor(glm::vec4 color);

private:
	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	glm::vec4 mColor;
	glm::vec3 mSize;
};