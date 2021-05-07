#pragma once

#include "glm/glm.hpp"

struct ShaderSourceCode
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
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

	Shader();
	Shader(const std::string& filePath);

	//-----------------------------------------------
	//		Initialize
	//
	
	void Initialize(const std::string& filePath);

	//-----------------------------------------------
	//		Destructors
	//

	~Shader();

	//-----------------------------------------------
	//		Uniforms
	//

	void SetUniform1i(const std::string& name, int v);
	void SetUniform1f(const std::string& name, float v);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	//-----------------------------------------------
	//		Else
	//

	void Bind() const;
	void Unbind() const;

private:
	/////////////////////////////////////////////////
	// 
	//		Methods
	//
	/////////////////////////////////////////////////

	ShaderSourceCode ReadShader(const std::string& filePath);
	unsigned int CreateShader(const std::string& vertexShader, const::std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);

	//-----------------------------------------------
	//		Uniforms
	//

	unsigned int ExtractUniformLocation(const std::string& name);

	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	unsigned int mRenderId;
	std::string mFilePath;
	std::unordered_map<std::string, int> mUniformLocationCache;
};
