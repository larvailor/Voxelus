#pragma once

#include "BaseComponent.h"

#include "Shader.h"

class MaterialComponent : public BaseComponent
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

	MaterialComponent(unsigned int entityId);

	//-----------------------------------------------
	//		Destructors
	//

	~MaterialComponent();

	//-----------------------------------------------
	//		Getters
	//

	Shader& GetShader();

	static ComponentType GetStaticComponentType();

private:
	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	Shader mShader; // TODO : shader should be a part of Material class
};
