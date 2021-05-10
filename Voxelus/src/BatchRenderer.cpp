#include "stdfax.h"

#include "BatchRenderer.h"

//#include "VertexArray.h"
//#include "DynamicVertexBuffer.h"
//#include "VertexBufferLayout.h"
//#include "IndexBuffer.h"

namespace
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct BatchRendererData
	{
		//VertexArray va;
		//DynamicVertexBuffer dvb;
		//VertexBufferLayout vbl;
		//IndexBuffer ib;

		unsigned int va;
		unsigned int dvb;
		unsigned int ib;

		Vertex* bufferStart = nullptr;
		Vertex* bufferCurr = nullptr;

		unsigned int usedIndicesCount = 0;
		unsigned int* indices = nullptr;
	};
	BatchRendererData batchRenderData;
}

void BatchRenderer::Init()
{
	batchRenderData.bufferStart = new Vertex[InitConstants::World::MaxVerticesCount];
	batchRenderData.indices = new unsigned int[InitConstants::World::MaxIndicesCount];

	//batchRenderData.dvb.Init(InitConstants::World::MaxVerticesCount * sizeof(Vertex));
	//batchRenderData.vbl.Push<float>(3);
	//batchRenderData.vbl.Push<float>(4);
	//batchRenderData.va.AddDynamicBuffer(batchRenderData.dvb, batchRenderData.vbl);

	// Vertex array
	GLCall(glCreateVertexArrays(1, &batchRenderData.va));
	GLCall(glBindVertexArray(batchRenderData.va));

	// Dynamic vertex buffer
	GLCall(glCreateBuffers(1, &batchRenderData.dvb));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, batchRenderData.dvb));
	GLCall(glBufferData(GL_ARRAY_BUFFER, InitConstants::World::MaxVerticesCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

	// Layout
	GLCall(glEnableVertexArrayAttrib(batchRenderData.va, 0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position)));

	GLCall(glEnableVertexArrayAttrib(batchRenderData.va, 1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color)));


	unsigned int offset = 0;
	//	    4 - - -  5
	//	  / .      / |
	//	7  -.- - 6   |
	//	|   0 . .|.  1
	//	| .`     | /
	//	3 - - -  2
	for (unsigned int i = 0; i < InitConstants::World::MaxIndicesCount; i += InitConstants::Voxel::IndicesPerVoxel)
	{
		// back
		batchRenderData.indices[i + 0] = 1 + offset;
		batchRenderData.indices[i + 1] = 0 + offset;
		batchRenderData.indices[i + 2] = 4 + offset;

		batchRenderData.indices[i + 3] = 4 + offset;
		batchRenderData.indices[i + 4] = 5 + offset;
		batchRenderData.indices[i + 5] = 1 + offset;

		// front
		batchRenderData.indices[i + 6] = 3 + offset;
		batchRenderData.indices[i + 7] = 2 + offset;
		batchRenderData.indices[i + 8] = 6 + offset;

		batchRenderData.indices[i + 9] = 6 + offset;
		batchRenderData.indices[i + 10] = 7 + offset;
		batchRenderData.indices[i + 11] = 3 + offset;

		// left
		batchRenderData.indices[i + 12] = 0 + offset;
		batchRenderData.indices[i + 13] = 3 + offset;
		batchRenderData.indices[i + 14] = 7 + offset;

		batchRenderData.indices[i + 15] = 7 + offset;
		batchRenderData.indices[i + 16] = 4 + offset;
		batchRenderData.indices[i + 17] = 0 + offset;

		// right
		batchRenderData.indices[i + 18] = 2 + offset;
		batchRenderData.indices[i + 19] = 1 + offset;
		batchRenderData.indices[i + 20] = 5 + offset;

		batchRenderData.indices[i + 21] = 5 + offset;
		batchRenderData.indices[i + 22] = 6 + offset;
		batchRenderData.indices[i + 23] = 2 + offset;

		// bottom
		batchRenderData.indices[i + 24] = 0 + offset;
		batchRenderData.indices[i + 25] = 1 + offset;
		batchRenderData.indices[i + 26] = 2 + offset;

		batchRenderData.indices[i + 27] = 2 + offset;
		batchRenderData.indices[i + 28] = 3 + offset;
		batchRenderData.indices[i + 29] = 0 + offset;

		// top
		batchRenderData.indices[i + 30] = 7 + offset;
		batchRenderData.indices[i + 31] = 6 + offset;
		batchRenderData.indices[i + 32] = 5 + offset;

		batchRenderData.indices[i + 33] = 5 + offset;
		batchRenderData.indices[i + 34] = 4 + offset;
		batchRenderData.indices[i + 35] = 7 + offset;

		offset += InitConstants::Voxel::VerticesPerVoxel;
	}

	// Index buffer
	GLCall(glCreateBuffers(1, &batchRenderData.ib));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchRenderData.ib));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, InitConstants::World::MaxIndicesCount, batchRenderData.indices, GL_STATIC_DRAW));
}

void BatchRenderer::DeInit()
{
	delete[] batchRenderData.bufferStart;
	delete[] batchRenderData.indices;
}

void BatchRenderer::BeginBatch()
{
	batchRenderData.bufferCurr = batchRenderData.bufferStart;
}

void BatchRenderer::EndBatch()
{
	GLsizeiptr size = (uint8_t*)batchRenderData.bufferCurr - (uint8_t*)batchRenderData.bufferStart;
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, batchRenderData.dvb));
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, batchRenderData.bufferStart);
}

void BatchRenderer::Flush()
{
	GLCall(glBindVertexArray(batchRenderData.va));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchRenderData.ib));
	glDrawElements(GL_TRIANGLES, batchRenderData.usedIndicesCount, GL_UNSIGNED_INT, nullptr);

	batchRenderData.usedIndicesCount = 0;
}

void BatchRenderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
{
	if (batchRenderData.usedIndicesCount >= InitConstants::World::MaxIndicesCount)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}


	//	    4 - - -  5
	//	  / .      / |
	//	7  -.- - 6   |
	//	|   0 . .|.  1
	//	| .`     | /
	//	3 - - -  2

	// 0
	batchRenderData.bufferCurr->Position = glm::vec3(position.x, position.y, position.z);
	batchRenderData.bufferCurr->Color = color;
	batchRenderData.bufferCurr++;

	// 1
	batchRenderData.bufferCurr->Position = glm::vec3(position.x + size.x, position.y, position.z);
	batchRenderData.bufferCurr->Color = color;
	batchRenderData.bufferCurr++;

	// 2
	batchRenderData.bufferCurr->Position = glm::vec3(position.x + size.x, position.y, position.z + size.z);
	batchRenderData.bufferCurr->Color = color;
	batchRenderData.bufferCurr++;

	// 3
	batchRenderData.bufferCurr->Position = glm::vec3(position.x, position.y, position.z + size.z);
	batchRenderData.bufferCurr->Color = color;
	batchRenderData.bufferCurr++;

	// 4
	batchRenderData.bufferCurr->Position = glm::vec3(position.x, position.y + size.y, position.z);
	batchRenderData.bufferCurr->Color = color;
	batchRenderData.bufferCurr++;

	// 5
	batchRenderData.bufferCurr->Position = glm::vec3(position.x + size.x, position.y + size.y, position.z);
	batchRenderData.bufferCurr->Color = color;
	batchRenderData.bufferCurr++;

	// 6
	batchRenderData.bufferCurr->Position = glm::vec3(position.x + size.x, position.y + size.y, position.z + size.z);
	batchRenderData.bufferCurr->Color = color;
	batchRenderData.bufferCurr++;

	// 7
	batchRenderData.bufferCurr->Position = glm::vec3(position.x, position.y + size.y, position.z + size.z);
	batchRenderData.bufferCurr->Color = color;
	batchRenderData.bufferCurr++;

	batchRenderData.usedIndicesCount += InitConstants::Voxel::IndicesPerVoxel;
}
