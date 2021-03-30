#include "Entity.h"

#include <iostream>

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
