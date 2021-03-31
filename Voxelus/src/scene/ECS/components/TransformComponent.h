#pragma once

#include "BaseComponent.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class TransformComponent : public BaseComponent
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

	TransformComponent(unsigned int entityId);

	//-----------------------------------------------
	//		Destructors
	//

	~TransformComponent();

	//-----------------------------------------------
	//		Getters
	//

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;

	glm::mat4 GetTransformMat() const;

	static ComponentType GetStaticComponentType();

	//-----------------------------------------------
	//		Setter
	//

	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec3& rotation);
	void SetScale(glm::vec3 scale);

private:
	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	glm::vec3 mPosition;
	glm::vec3 mRotation;
	glm::vec3 mScale;
};
