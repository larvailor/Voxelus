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
	
	glm::vec3 GetColor() const;
	
	//-----------------------------------------------
	//		Setters
	//

	void SetColor(glm::vec3 color);

private:
	glm::vec3 mColor;
};