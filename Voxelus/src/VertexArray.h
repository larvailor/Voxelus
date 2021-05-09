#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "DynamicVertexBuffer.h"

class VertexArray
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

	VertexArray();

	//-----------------------------------------------
	//		Destructors
	//

	~VertexArray();

	//-----------------------------------------------
	//		Else
	//

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void AddDynamicBuffer(const DynamicVertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

private:
	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	unsigned int mRendererId;
};