#pragma once

#include "Entity.h"

class Ray;

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
	void SetBaseColor(glm::vec4 color);
	void SetSize(glm::vec3 size);
	void SetHovered(bool isHovered);
	void SetSelected(bool isSelected);

	//-----------------------------------------------
	//		Collisions
	//

	bool Intersects(glm::vec3 position);

private:
	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	glm::vec4 mColor;
	glm::vec4 mBaseColor;
	glm::vec4 mHoveredColor;

	glm::vec3 mSize;

	bool mIsHovered;
	bool mIsSelected;
};