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
	/////////////////////////////////////////////////
	// 
	//		Methods
	//
	/////////////////////////////////////////////////
	
	//-----------------------------------------------
	//		Constructors
	//

	Shader(const std::string& filePath);
	
	//-----------------------------------------------
	//		Destructors
	//

	~Shader();

	//-----------------------------------------------
	//		Uniforms
	//

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	//-----------------------------------------------
	//		Else
	//

	void Bind();
	void Unbind();

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
