#pragma once

class Texture
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
		
	Texture(const std::string& path);

	//-----------------------------------------------
	//		Destructors
	//

	~Texture();

	//-----------------------------------------------
	//		Getters
	//

	int GetWidth() const;
	int GetHeight() const;

	//-----------------------------------------------
	//		Else
	//

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

private:
	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	unsigned int mRendererId;
	std::string mFilePath;
	unsigned char* mLocalBuffer;
	int mWidth;
	int mHeight;
	int mBPP;
};
