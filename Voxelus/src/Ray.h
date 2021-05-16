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
	Ray(glm::vec3 origin, glm::vec3 direction, float length);

	//-----------------------------------------------
	//		Getters
	//

	glm::vec3 GetOrigin() const;
	glm::vec3 GetDirection() const;
	float GetLength() const;

private:
	/////////////////////////////////////////////////
	// 
	//		Veriables
	//
	/////////////////////////////////////////////////

	glm::vec3 mOrigin;
	glm::vec3 mDirection;
	float mLength;
};
