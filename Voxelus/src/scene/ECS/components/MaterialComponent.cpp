#include "stdfax.h"

#include "MaterialComponent.h"

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

MaterialComponent::MaterialComponent(unsigned int entityId)
	: BaseComponent(entityId, ComponentType::Material)
{

}

//-----------------------------------------------
//		Destructors
//

MaterialComponent::~MaterialComponent()
{
}

//-----------------------------------------------
//		Getters
//

Shader& MaterialComponent::GetShader()
{
	return mShader;
}

ComponentType MaterialComponent::GetStaticComponentType()
{
	return ComponentType::Material;
}
