#include "IndexBuffer.h"

#include "Renderer.h"

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: mCount(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &mRendererId));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererId));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

//-----------------------------------------------
//		Destructors
//

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &mRendererId));
}

//-----------------------------------------------
//		Getters
//

unsigned int IndexBuffer::GetCount() const
{
	return mCount;
}

//-----------------------------------------------
//		Else
//

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererId));

}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
