#pragma once

#include "BaseComponent.h"

#include "VertexArray.h"
#include "IndexBuffer.h"

class MeshComponent : public BaseComponent
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

	MeshComponent(unsigned int entityId);

	//-----------------------------------------------
	//		Destructors
	//

	virtual ~MeshComponent();

	//-----------------------------------------------
	//		Getters
	//

	VertexArray& GetVertexArray();
	IndexBuffer& GetIndexBuffer();

	static ComponentType GetStaticComponentType();

private:
	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	VertexArray mVertexArray;
	
	VertexBuffer mVertexBuffer;
	VertexBufferLayout mVertexBufferLayout;

	IndexBuffer mIndexBuffer;

	float mCubeVertices[20] = {
		0.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		30.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		30.0f, 30.0f, 0.0f, 1.0f, 1.0f,
		0.0f,  30.0f, 0.0f, 0.0f, 1.0f
		};

	unsigned int mCubeIndices[6] = {
		0, 1, 2,
		2, 3, 0
	};
};
