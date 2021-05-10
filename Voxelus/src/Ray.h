#pragma once

class Ray
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

	Ray();
	Ray(glm::vec3 origin, glm::vec3 direction);

	//-----------------------------------------------
	//		Getters
	//

	glm::vec3 GetOrigin() const;
	glm::vec3 GetDirection() const;

private:
	/////////////////////////////////////////////////
	// 
	//		Veriables
	//
	/////////////////////////////////////////////////

	glm::vec3 mOrigin;
	glm::vec3 mDirection;
};
