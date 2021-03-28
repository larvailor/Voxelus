#pragma once

class VertexBuffer
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

	VertexBuffer(const void* data, unsigned int size);

	//-----------------------------------------------
	//		Destructors
	//

	~VertexBuffer();

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
