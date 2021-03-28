#include "Renderer.h"

#include <iostream>

void GlClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGl ERROR! " << error << " ] " << std::endl;
		std::cout << "          function: " << function << std::endl;
		std::cout << "          file:     " << file << std::endl;
		std::cout << "          line:     " << line << std::endl;
		return false;
	}
	return true;
}

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

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
	shader.Bind();
	//shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

	// т.к. забиндили vao и buffer, то остается просто забиндить здесь vao и ibo
	vertexArray.Bind();
	indexBuffer.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}
