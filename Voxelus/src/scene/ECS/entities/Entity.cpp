#include "stdfax.h"

#include "Entity.h"
#include "ECS/components/TransformComponent.h"

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

Entity::Entity()
{
	mId = NextEntityId;
	NextEntityId += 1;

	AddComponent<TransformComponent>();
}

//-----------------------------------------------
//		Destructors
//

Entity::~Entity()
{
	
}

//-----------------------------------------------
//		Getters
//

unsigned int Entity::GetId() const
{
	return mId;
}
