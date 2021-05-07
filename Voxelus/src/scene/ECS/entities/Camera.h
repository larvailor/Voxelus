#pragma once

#include "Entity.h"

class Camera : public Entity
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

	Camera();

	//-----------------------------------------------
	//		Destructors
	//

	~Camera();
	
	//-----------------------------------------------
	//		Getters
	//

	glm::mat4& GetViewMatrix();
	glm::mat4& GetProjectionMatrix();

	//-----------------------------------------------
	//		Update
	//

	void OnTickUpdate();
	
	//-----------------------------------------------
	//		Handlers
	//

	void HandleInput(bool* keys);

private:
	glm::mat4 mCachedViewMatrix;
	glm::mat4 mCachedProjectionMatrix;
};
