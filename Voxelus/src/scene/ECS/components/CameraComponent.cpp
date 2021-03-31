#include "stdfax.h"

#include "CameraComponent.h"

namespace
{
	const float cStartNearPlane   = 1.0f;
	const float cStartFarPlane    = 1000.0f;
	const float cStartFieldOfView = 60.0f;

	const glm::vec3 cStartLookDirection = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
	const glm::vec3 cStartUp            = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));

	const ProjectionType cStartProjectionType = ProjectionType::Perspective;
}

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

CameraComponent::CameraComponent(unsigned int entityId)
	: mNearPlane(cStartNearPlane)
	, mFarPlane(cStartFarPlane)
	, mFieldOfView(cStartFieldOfView)
	, mLookDirection(cStartLookDirection)
	, mUp(cStartUp)
	, mProjectionType(cStartProjectionType)
	, BaseComponent(entityId, ComponentType::Transform)
{
}

//-----------------------------------------------
//		Destructors
//

CameraComponent::~CameraComponent()
{
}

//-----------------------------------------------
//		Getters
//

float CameraComponent::GetNearPlane() const
{
	return mNearPlane;
}

float CameraComponent::GetFarPlane() const
{
	return mFarPlane;
}

float CameraComponent::GetFieldOfView() const
{
	return mFieldOfView;
}

glm::vec3 CameraComponent::GetLookDirection() const
{
	return mLookDirection;
}

glm::vec3 CameraComponent::GetUp() const
{
	return mUp;
}

ProjectionType CameraComponent::GetProjectionType() const
{
	return mProjectionType;
}

glm::mat4 CameraComponent::GetProjectionMat() const
{
	return glm::perspective(glm::radians(mFieldOfView), 4.0f / 3.0f, mNearPlane, mFarPlane); // TODO: aspect ratio
}

ComponentType CameraComponent::GetStaticComponentType()
{
	return ComponentType::Camera;
}

//-----------------------------------------------
//		Setter
//

void CameraComponent::SetNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
}

void CameraComponent::SetFarPlane(float farPlane)
{
	mFarPlane = farPlane;
}

void CameraComponent::SetFieldOfView(float fieldOfView)
{
	mFieldOfView = fieldOfView;
}

void CameraComponent::SetLookDirection(glm::vec3 lookDirection)
{
	mLookDirection = lookDirection;
}

void CameraComponent::SetUp(glm::vec3 up)
{
	mUp = up;
}

void CameraComponent::SetProjectionType(ProjectionType projectionType)
{
	mProjectionType = projectionType;
}
