#pragma once

#include "BaseComponent.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class ProjectionType
{
	Orthographical,
	Perspective
};

class CameraComponent : public BaseComponent
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

	CameraComponent(unsigned int entityId);

	//-----------------------------------------------
	//		Destructors
	//

	~CameraComponent();

	//-----------------------------------------------
	//		Getters
	//

	float GetNearPlane() const;
	float GetFarPlane() const;
	float GetFieldOfView() const;

	glm::vec3 GetLookDirection() const;
	glm::vec3 GetUp() const;

	ProjectionType GetProjectionType() const;

	glm::mat4 GetProjectionMat() const;

	static ComponentType GetStaticComponentType();

	//-----------------------------------------------
	//		Setters
	//

	void SetNearPlane(float newarPlane);
	void SetFarPlane(float farPlane);
	void SetFieldOfView(float fieldOfView);

	void SetLookDirection(glm::vec3 lookDirection);
	void SetUp(glm::vec3 up);

	void SetProjectionType(ProjectionType projectionType);

private:
	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	float mNearPlane;
	float mFarPlane;
	float mFieldOfView;

	glm::vec3 mLookDirection;
	glm::vec3 mUp;

	ProjectionType mProjectionType;
};