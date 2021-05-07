#include "stdfax.h"

#include "Camera.h"

#include "ECS/components/Components.h"

namespace
{
	const glm::vec3 StartPosition = glm::vec3(0.0f, 0.0f, 100.0f);

	const float MovementSpeed = 1.5f;
	const float ShiftMultiplier = 2.0f;
	const float RotationSpeed = 1.0f;
}

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

Camera::Camera()
	: Entity()
{
	mCachedViewMatrix = glm::mat4(1.0f);
	mCachedProjectionMatrix = glm::mat4(1.0f);

	AddComponent<CameraComponent>();
	GetComponent<TransformComponent>()->SetPosition(StartPosition);
}

//-----------------------------------------------
//		Destructors
//

Camera::~Camera()
{
}

//-----------------------------------------------
//		Getters
//

glm::mat4& Camera::GetViewMatrix()
{
	return mCachedViewMatrix;
}

glm::mat4& Camera::GetProjectionMatrix()
{
	return mCachedProjectionMatrix;
}

//-----------------------------------------------
//		Update
//

void Camera::OnTickUpdate()
{
	// Update cache
	mCachedViewMatrix = glm::lookAt(GetComponent<TransformComponent>()->GetPosition(), 
		GetComponent<TransformComponent>()->GetPosition() + GetComponent<CameraComponent>()->GetLookDirection(), 
		GetComponent<CameraComponent>()->GetUp());

	mCachedProjectionMatrix = GetComponent<CameraComponent>()->GetProjectionMat();

	// Update math
	std::shared_ptr<TransformComponent> transformComp = GetComponent<TransformComponent>();
	std::shared_ptr<CameraComponent> cameraComp = GetComponent<CameraComponent>();
	//cameraComp->SetLookDirection(glm::rotate(cameraComp->GetLookDirection(), transformComp->GetRotation()));
}

//-----------------------------------------------
//		Handlers
//

void Camera::HandleInput(bool* keys)
{
	float cameraSpeed = MovementSpeed;
	if (keys[GLFW_KEY_LEFT_SHIFT])
		cameraSpeed *= ShiftMultiplier;

	std::shared_ptr<TransformComponent> transformComp = GetComponent<TransformComponent>();
	std::shared_ptr<CameraComponent> cameraComp = GetComponent<CameraComponent>();
	if (keys[GLFW_KEY_W])
		transformComp->SetPosition(transformComp->GetPosition() + cameraSpeed * cameraComp->GetLookDirection());
	if (keys[GLFW_KEY_S])
		transformComp->SetPosition(transformComp->GetPosition() - cameraSpeed * cameraComp->GetLookDirection());
	if (keys[GLFW_KEY_A])
		transformComp->SetPosition(transformComp->GetPosition() - glm::normalize(glm::cross(cameraComp->GetLookDirection(), cameraComp->GetUp())) * cameraSpeed);
	if (keys[GLFW_KEY_D])
		transformComp->SetPosition(transformComp->GetPosition() + glm::normalize(glm::cross(cameraComp->GetLookDirection(), cameraComp->GetUp())) * cameraSpeed);

	if (keys[GLFW_KEY_Z])
		transformComp->SetPosition(transformComp->GetPosition() - cameraSpeed * cameraComp->GetUp());
	if (keys[GLFW_KEY_X])
		transformComp->SetPosition(transformComp->GetPosition() + cameraSpeed * cameraComp->GetUp());

	float rotationSpeed = 1.0f; // deg
	if (keys[GLFW_KEY_Q])
	{
		glm::vec3 currentRotation = transformComp->GetRotation();
		currentRotation.y += rotationSpeed;
		currentRotation.y = static_cast<int>(currentRotation.y) % 365;
		transformComp->SetRotation(currentRotation);
	}
	//cameraTransformComp->SetPosition(cameraTransformComp->GetPosition() - cameraSpeed * cameraCameraComp->GetUp());
	//if (mKeys[GLFW_KEY_E])
	//	cameraTransformComp->SetPosition(cameraTransformComp->GetPosition() + cameraSpeed * cameraCameraComp->GetUp());
}


