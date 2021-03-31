#include "stdfax.h"

#include "BaseComponent.h"

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Destructors
//

BaseComponent::~BaseComponent()
{

}

//-----------------------------------------------
//		Getters
//

unsigned int BaseComponent::GetEntityId() const
{
    return mEntityId;
}

ComponentType BaseComponent::GetComponentType() const
{
    return mComponentType;
}

//-----------------------------------------------
//		Setters
//

void BaseComponent::SetEntityId(unsigned int entityId)
{
    mEntityId = entityId;
}

/////////////////////////////////////////////////
// 
//		PROTECTED METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

BaseComponent::BaseComponent(unsigned int entityId, ComponentType componentType)
    : mEntityId(entityId)
    , mComponentType(componentType)
{

}
