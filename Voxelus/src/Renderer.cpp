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