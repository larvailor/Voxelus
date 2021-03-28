#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GlClearError();\
	x;\
	ASSERT(GlLogCall(#x, __FILE__, __LINE__))

void GlClearError();
bool GlLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	/////////////////////////////////////////////////
	// 
	//		Methods
	//
	/////////////////////////////////////////////////

	//-----------------------------------------------
	//		Drawing
	//

	void Clear() const;
	void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const;
};
