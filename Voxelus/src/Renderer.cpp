#include "stdfax.h"

#include "Renderer.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Drawing
//

void Renderer::Clear() const
{
	GLCall(glClearColor(0.8f, 0.8f, 0.8f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer) const
{
	vertexArray.Bind();
	indexBuffer.Bind();

	GLCall(glDrawElements(GL_TRIANGLE_STRIP, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}
