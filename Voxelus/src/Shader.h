#pragma once

#include <string>
#include <unordered_map>

struct ShaderSourceCode
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind();
	void Unbind();

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:

	ShaderSourceCode ReadShader(const std::string& filePath);
	unsigned int CreateShader(const std::string& vertexShader, const::std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);

	unsigned int GetUniformLocation(const std::string& name);

	unsigned int mRenderId;
	std::string mFilePath;
	std::unordered_map<std::string, int> mUniformLocationCache;
};
