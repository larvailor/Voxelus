#pragma once

#include "Entity.h"

class Ray;


class Voxel : public Entity
{
public:
	enum class HoveredSide
	{
		None = -1,
		Back,
		Front,
		Left,
		Right,
		Bottom,
		Top
	};

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
	HoveredSide GetHoveredSide() const;
	
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

	HoveredSide GetPlaneNumberThatIntersectsWithRay(const Ray& ray, const glm::vec3& currRayPosition);

	bool IsFloorPart = false;

private:
	/////////////////////////////////////////////////
	// 
	//		Methods
	//
	/////////////////////////////////////////////////
	 
	//-----------------------------------------------
	//		Collisions
	//

	bool RayPlaneIntersection(const glm::vec3& planeNormal, const glm::vec3& planePoint, const Ray& ray);

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
	HoveredSide mHoveredSide = HoveredSide::None;

	bool mIsSelected;

	// back
	glm::vec3 backPanelNormal = glm::vec3(0.0f, 0.0f, -1.0f);
	// front
	glm::vec3 frontPanelNormal = glm::vec3(0.0f, 0.0f, 1.0f);
	// left
	glm::vec3 leftPanelNormal = glm::vec3(-1.0f, 0.0f, 0.0f);
	// right
	glm::vec3 rightPanelNormal = glm::vec3(1.0f, 0.0f, 0.0f);
	// bottom
	glm::vec3 bottomPanelNormal = glm::vec3(0.0f, -1.0f, 0.0f);
	// top
	glm::vec3 topPanelNormal = glm::vec3(0.0f, 1.0f, 0.0f);
};