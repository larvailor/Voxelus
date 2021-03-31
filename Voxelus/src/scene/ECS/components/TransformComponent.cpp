#include "stdfax.h"

#include "TransformComponent.h"

namespace
{
	const glm::vec3 cStartPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 cStartRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 cStartScale    = glm::vec3(1.0f, 1.0f, 1.0f);
}

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

TransformComponent::TransformComponent(unsigned int entityId)
	: mPosition(cStartPosition)
	, mRotation(cStartRotation)
	, mScale(cStartScale)
	, BaseComponent(entityId, ComponentType::Transform)
{
}

//-----------------------------------------------
//		Destructors
//

TransformComponent::~TransformComponent()
{

}

//-----------------------------------------------
//		Getters
//

glm::vec3 TransformComponent::GetPosition() const
{
	return mPosition;
}

glm::vec3 TransformComponent::GetRotation() const
{
	return mRotation;
}

glm::vec3 TransformComponent::GetScale() const
{
	return mScale;
}

glm::mat4 TransformComponent::GetTransformMat() const
{
	glm::mat4 transformMat(1.0f);
	transformMat = glm::scale(transformMat, mScale);

	// Euler Angles. Y -> Z -> X
	transformMat = glm::rotate(transformMat, glm::radians(mRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transformMat = glm::rotate(transformMat, glm::radians(mRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	transformMat = glm::rotate(transformMat, glm::radians(mRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	transformMat = glm::translate(transformMat, mPosition);

	return transformMat;
}

ComponentType TransformComponent::GetStaticComponentType()
{
	return ComponentType::Transform;
}

//-----------------------------------------------
//		Setters
//

void TransformComponent::SetPosition(const glm::vec3& position)
{
	mPosition = position;
}

void TransformComponent::SetRotation(const glm::vec3& rotation)
{
	mRotation = rotation;
}

void TransformComponent::SetScale(glm::vec3 scale)
{
	mScale = scale;
}
