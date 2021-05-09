#pragma once

class DynamicVertexBuffer
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

	DynamicVertexBuffer();
	DynamicVertexBuffer(unsigned int size);

	//-----------------------------------------------
	//		Destructors
	//

	~DynamicVertexBuffer();

	//-----------------------------------------------
	//		Initialization
	//

	void Init(unsigned int size);
	
	//-----------------------------------------------
	//		Setters
	//

	void SetBufferSubData(unsigned int size, const void* data);
	
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
};