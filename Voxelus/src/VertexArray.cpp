#include "stdfax.h"

#include "VertexArray.h"
#include "Renderer.h"

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &mRendererId));
}

//-----------------------------------------------
//		Destructors
//

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &mRendererId));
}

//-----------------------------------------------
//		Else
//

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];

		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.isNormalized, layout.GetStride(), (const void*)offset)); // ��������� buffer � vao : ������ 0 vao �������� �� buffer
		// ���� � ���� ��������� ������ �����, �� ������ ����� 1. ����� ��� ����� glEnableVertexAttribArray(1)
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(mRendererId));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
