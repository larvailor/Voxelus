#pragma once

#include "Renderer.h"

#include <GL/glew.h>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char isNormalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return sizeof(GLfloat);
		case GL_UNSIGNED_INT:
			return sizeof(GLuint);
		case GL_UNSIGNED_BYTE:
			return sizeof(GLbyte);
		}
		ASSERT(false);
		return 0;
	}
};



class VertexBufferLayout
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

	VertexBufferLayout()
		: mStride(0)
	{

	}

	//-----------------------------------------------
	//		Destructors
	//

	// ...

	//-----------------------------------------------
	//		Getters
	//

	const std::vector<VertexBufferElement>& GetElements() const
	{
		return mElements;
	}

	unsigned int GetStride() const
	{
		return mStride;
	}
	
	//-----------------------------------------------
	//		Setters
	//

	// ...

	//-----------------------------------------------
	//		Else
	//

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		mElements.push_back({ GL_FLOAT, count, GL_FALSE });
		mStride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		mElements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		mStride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		mElements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		mStride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

private:
	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////
	
	std::vector<VertexBufferElement> mElements;
	unsigned int mStride;
};
