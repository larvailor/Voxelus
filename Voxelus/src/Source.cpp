#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct ShaderSourceCode
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderSourceCode ReadShader(const std::string& shaderPath)
{
	std::ifstream stream(shaderPath);


	enum class ShaderType
	{
		None = -1,
		Vertex,
		Fragment
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType shaderType = ShaderType::None;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				shaderType = ShaderType::Vertex;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				shaderType = ShaderType::Fragment;
			}
		}
		else
		{
			ss[static_cast<int>(shaderType)] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}



static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int shader = glCreateShader(type);
	const char* src = source.c_str();
	GLCall(glShaderSource(shader, 1, &src, nullptr));
	GLCall(glCompileShader(shader));

	int result;
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(shader, length, &length, message));
		std::cout << "ERROR! CompileShader failed for " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(shader);)
		return 0;
	}

	return shader;
}



static unsigned int CreateShader(const std::string& vertexShader, const::std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}



int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR! glewInit() failed" << std::endl;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		float positions[] = {
			-0.5f, -0.5f, // 0
			 0.5f, -0.5f, // 1
			 0.5f,  0.5f, // 2
			-0.5f,  0.5f  // 3
		};
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		VertexBuffer vertexBugger(positions, 4 * 2 * sizeof(float));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0)); // соединяет buffer с vao : индекс 0 vao биндится на buffer
		// если я хочу забиндить другой буфер, то индекс будет 1. также его нужно glEnableVertexAttribArray(1)

		IndexBuffer indexBuffer(indices, 6);

		ShaderSourceCode shaderSourceCode = ReadShader("res/shaders/Basic.shader");
		unsigned int shader = CreateShader(shaderSourceCode.VertexSource, shaderSourceCode.FragmentSource);
		GLCall(glUseProgram(shader));

		GLCall(int location = glGetUniformLocation(shader, "u_Color"));
		ASSERT(location != -1);

		GLCall(glBindVertexArray(0));
		GLCall(glUseProgram(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		float r = 0.0f;
		float increment = 0.01f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			GLCall(glUseProgram(shader));
			GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

			// т.к. забиндили vao и buffer, то остается просто забиндить здесь vao и ibo
			GLCall(glBindVertexArray(vao));
			indexBuffer.Bind();

			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			if (r > 1.0f || r < 0.0f)
			{
				increment *= -1;
			}
			r += increment;

			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			GLCall(glfwPollEvents());
		}

		GLCall(glDeleteProgram(shader));
	}

	glfwTerminate();
	return 0;
}