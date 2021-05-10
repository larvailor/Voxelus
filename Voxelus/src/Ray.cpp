#include "stdfax.h"

#include "Ray.h"

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

Ray::Ray()
	: mOrigin(glm::vec3(0.0f, 0.0f, 0.0f))
	, mDirection(glm::vec3(0.0f, 0.0f, -1.0f))
{
}

Ray::Ray(glm::vec3 origin, glm::vec3 direction)
	: mOrigin(origin)
	, mDirection(direction)
{
}

//-----------------------------------------------
//		Getters
//

glm::vec3 Ray::GetOrigin() const
{
	return mOrigin;
}

glm::vec3 Ray::GetDirection() const
{
	return mDirection;
}
