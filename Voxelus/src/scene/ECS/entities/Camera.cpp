#include "stdfax.h"

#include "Camera.h"

#include "ECS/components/Components.h"

namespace
{
	const glm::vec3 StartPosition = glm::vec3(320.0f, 250.0f, 600.0f);

	const float MovementSpeed = 150.0f;
	const float ShiftMultiplier = 4.0f;
	const float RotationDegSpeed = 120.0f;

	const float StartYaw = -90.0f;
	const float StartPitch = -42.0f;
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
	: mYaw(StartYaw)
	, mPitch(StartPitch)
	, Entity()
{
	mCachedViewMatrix = glm::mat4(1.0f);
	mCachedProjectionMatrix = glm::mat4(1.0f);

	GetComponent<TransformComponent>()->SetPosition(StartPosition);

	AddComponent<CameraComponent>();

	glm::vec3 newLookDirection;
	newLookDirection.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	newLookDirection.y = sin(glm::radians(mPitch));
	newLookDirection.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	GetComponent<CameraComponent>()->SetLookDirection(glm::normalize(newLookDirection));
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
}

//-----------------------------------------------
//		Handlers
//

void Camera::HandleInput(bool* keys)
{
	float cameraSpeed = MovementSpeed * Time::DeltaTime;
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

	float rotationSpeed = RotationDegSpeed * Time::DeltaTime;
	if (keys[GLFW_KEY_Q])
	{
		mYaw -= rotationSpeed;
		glm::vec3 newLookDirection;
		newLookDirection.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		newLookDirection.y = sin(glm::radians(mPitch));
		newLookDirection.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		cameraComp->SetLookDirection(glm::normalize(newLookDirection));
	}
	if (keys[GLFW_KEY_E])
	{
		mYaw += rotationSpeed;
		glm::vec3 newLookDirection;
		newLookDirection.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		newLookDirection.y = sin(glm::radians(mPitch));
		newLookDirection.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		cameraComp->SetLookDirection(glm::normalize(newLookDirection));
	}


	if (Mouse::Button == GLFW_MOUSE_BUTTON_MIDDLE && Mouse::Action == GLFW_PRESS)
	{
		// Update rotation
		mYaw += static_cast<float>(Mouse::OffsetX);
		mPitch += static_cast<float>(Mouse::OffsetY);

		if (mPitch > 89.0f)
		{
			mPitch = 89.0f;
		}
		if (mPitch < -89.0f)
		{
			mPitch = -89.0f;
		}
		glm::vec3 newLookDirection;
		newLookDirection.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		newLookDirection.y = sin(glm::radians(mPitch));
		newLookDirection.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		cameraComp->SetLookDirection(glm::normalize(newLookDirection));

		Mouse::OffsetX = 0.0;
		Mouse::OffsetY = 0.0;
	}
}


