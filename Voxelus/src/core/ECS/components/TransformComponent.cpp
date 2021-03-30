#include "TransformComponent.h"

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

TransformComponent::TransformComponent(unsigned int entityId)
	: mPosition(0.0f, 0.0f, 0.0f)
	, mRotation(0.0f, 0.0f, 0.0f)
	, mScale(1.0f, 1.0f, 1.0f)
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
	// TODO: add rotations support
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
