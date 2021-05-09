#include "stdfax.h"

#include "DynamicVertexBuffer.h"

#include "Renderer.h"

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

DynamicVertexBuffer::DynamicVertexBuffer()
{
	GLCall(glGenBuffers(1, &mRendererId));
}

DynamicVertexBuffer::DynamicVertexBuffer(unsigned int size)
{
	GLCall(glGenBuffers(1, &mRendererId));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererId));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
}

//-----------------------------------------------
//		Destructors
//

DynamicVertexBuffer::~DynamicVertexBuffer()
{
}

//-----------------------------------------------
//		Initialization
//

void DynamicVertexBuffer::Init(unsigned int size)
{
	Bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
}

//-----------------------------------------------
//		Setters
//

void DynamicVertexBuffer::SetBufferSubData(unsigned int size, const void* data)
{
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

//-----------------------------------------------
//		Else
//

void DynamicVertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererId));
}

void DynamicVertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
