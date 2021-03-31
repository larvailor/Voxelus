#include "stdfax.h"

#include "VertexBuffer.h"

#include "Renderer.h"

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCall(glGenBuffers(1, &mRendererId));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererId));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

//-----------------------------------------------
//		Destructors
//

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, & mRendererId));
}

//-----------------------------------------------
//		Else
//

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererId));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
