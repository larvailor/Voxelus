#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& filePath)
	: mFilePath(filePath)
	, mRenderId(0)
{
	ShaderSourceCode shaderSourceCode = ReadShader(filePath);
	mRenderId = CreateShader(shaderSourceCode.VertexSource, shaderSourceCode.FragmentSource);
}



Shader::~Shader()
{
	GLCall(glDeleteProgram(mRenderId));
}



void Shader::Bind()
{
	GLCall(glUseProgram(mRenderId));
}



void Shader::Unbind()
{
	GLCall(glUseProgram(0));
}



void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}



unsigned int Shader::GetUniformLocation(const std::string& name)
{
	if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
	{
		return mUniformLocationCache[name];
	}

	GLCall(int location = glGetUniformLocation(mRenderId, name.c_str()));
	if (location == -1)
	{
		std::cout << "WARNING! Uniform [" << name << "] not found" << std::endl;
	}
	mUniformLocationCache[name] = location;
	return location;
}



ShaderSourceCode Shader::ReadShader(const std::string& filePath)
{
	std::ifstream stream(filePath);
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

unsigned int Shader::CreateShader(const std::string& vertexShader, const::std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
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



unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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
