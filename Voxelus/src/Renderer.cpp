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
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer) const
{
	vertexArray.Bind();
	indexBuffer.Bind();

	GLCall(glDrawElements(GL_LINES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}
