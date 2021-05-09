#pragma once

class IndexBuffer
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

	IndexBuffer();
	IndexBuffer(const unsigned int* data, unsigned int count);
	
	//-----------------------------------------------
	//		Destructors
	//
	
	~IndexBuffer();

	//-----------------------------------------------
	//		Initialization
	//

	void Init(const unsigned int* data, unsigned int count);

	//-----------------------------------------------
	//		Getters
	//

	unsigned int GetCount() const;

	//-----------------------------------------------
	//		Else
	//	

	void Bind() const;
	void Unbind() const;

private:
	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	unsigned int mRendererId;
	unsigned int mCount;
};
