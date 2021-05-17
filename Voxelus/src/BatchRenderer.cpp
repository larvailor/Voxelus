#include "stdfax.h"

#include "BatchRenderer.h"

namespace
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct BatchRendererData
	{
		unsigned int va;
		unsigned int dvb;
		unsigned int ib;

		Vertex* bufferStart = nullptr;
		Vertex* bufferCurr = nullptr;

		unsigned int usedIndicesCount = 0;
		unsigned int* indices = nullptr;
	};
	BatchRendererData batchLineRenderData;
	BatchRendererData batchCubeRenderData;
}

/////////////////////////////////////////////////
// 
//		Line
//
/////////////////////////////////////////////////

void BatchLineRenderer::Init()
{
	batchLineRenderData.bufferStart = new Vertex[InitConstants::Line::MaxBatchVerticesCount];
	batchLineRenderData.indices = new unsigned int[InitConstants::Line::MaxBatchIndicesCount];

	// Vertex array
	GLCall(glCreateVertexArrays(1, &batchLineRenderData.va));
	GLCall(glBindVertexArray(batchLineRenderData.va));

	// Dynamic vertex buffer
	GLCall(glCreateBuffers(1, &batchLineRenderData.dvb));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, batchLineRenderData.dvb));
	GLCall(glBufferData(GL_ARRAY_BUFFER, InitConstants::Line::MaxBatchVerticesCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

	// Layout
	GLCall(glEnableVertexArrayAttrib(batchLineRenderData.va, 0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position)));

	GLCall(glEnableVertexArrayAttrib(batchLineRenderData.va, 1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color)));
	
	unsigned int offset = 0;
	for (unsigned int i = 0; i < InitConstants::Line::MaxBatchIndicesCount; i += InitConstants::Line::IndicesPerLine)
	{
		batchLineRenderData.indices[i + 0] = 0 + offset;
		batchLineRenderData.indices[i + 1] = 1 + offset;

		offset += InitConstants::Line::VerticesPerLine;
	}

	// Index buffer
	GLCall(glCreateBuffers(1, &batchLineRenderData.ib));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchLineRenderData.ib));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, InitConstants::Line::MaxBatchIndicesCount, batchLineRenderData.indices, GL_STATIC_DRAW));
}

void BatchLineRenderer::DeInit()
{
	delete[] batchLineRenderData.bufferStart;
	delete[] batchLineRenderData.indices;
}

void BatchLineRenderer::BeginBatch()
{
	batchLineRenderData.bufferCurr = batchLineRenderData.bufferStart;
}

void BatchLineRenderer::EndBatch()
{
	GLsizeiptr size = (uint8_t*)batchLineRenderData.bufferCurr - (uint8_t*)batchLineRenderData.bufferStart;
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, batchLineRenderData.dvb));
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, batchLineRenderData.bufferStart);
}

void BatchLineRenderer::Flush()
{
	GLCall(glBindVertexArray(batchLineRenderData.va));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchLineRenderData.ib));
	glDrawElements(GL_LINES, batchLineRenderData.usedIndicesCount, GL_UNSIGNED_INT, nullptr);

	batchLineRenderData.usedIndicesCount = 0;
}

void BatchLineRenderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4 color)
{
	//std::cout << start.x << " " << start.y << " " << start.z << std::endl;
	//std::cout << end.x << " " << end.y << " " << end.z << std::endl;
	//std::cout << std::endl;
	if (batchLineRenderData.usedIndicesCount >= InitConstants::Line::MaxBatchIndicesCount)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	// 0
	batchLineRenderData.bufferCurr->Position = glm::vec3(start.x, start.y, start.z);
	batchLineRenderData.bufferCurr->Color = color;
	batchLineRenderData.bufferCurr++;

	// 1
	batchLineRenderData.bufferCurr->Position = glm::vec3(end.x, end.y, end.z);
	batchLineRenderData.bufferCurr->Color = glm::vec4(0.0f, 0.5f, 0.0f, 0.5f);
	batchLineRenderData.bufferCurr++;

	batchLineRenderData.usedIndicesCount += InitConstants::Line::IndicesPerLine;
}

/////////////////////////////////////////////////
// 
//		Cube
//
/////////////////////////////////////////////////

void BatchCubeRenderer::Init()
{
	batchCubeRenderData.bufferStart = new Vertex[InitConstants::World::MaxVerticesCount];
	batchCubeRenderData.indices = new unsigned int[InitConstants::World::MaxIndicesCount];

	// Vertex array
	GLCall(glCreateVertexArrays(1, &batchCubeRenderData.va));
	GLCall(glBindVertexArray(batchCubeRenderData.va));

	// Dynamic vertex buffer
	GLCall(glCreateBuffers(1, &batchCubeRenderData.dvb));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, batchCubeRenderData.dvb));
	GLCall(glBufferData(GL_ARRAY_BUFFER, InitConstants::World::MaxVerticesCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

	// Layout
	GLCall(glEnableVertexArrayAttrib(batchCubeRenderData.va, 0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position)));

	GLCall(glEnableVertexArrayAttrib(batchCubeRenderData.va, 1));
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
		batchCubeRenderData.indices[i + 0] = 1 + offset;
		batchCubeRenderData.indices[i + 1] = 0 + offset;
		batchCubeRenderData.indices[i + 2] = 4 + offset;

		batchCubeRenderData.indices[i + 3] = 4 + offset;
		batchCubeRenderData.indices[i + 4] = 5 + offset;
		batchCubeRenderData.indices[i + 5] = 1 + offset;

		// front
		batchCubeRenderData.indices[i + 6] = 3 + offset;
		batchCubeRenderData.indices[i + 7] = 2 + offset;
		batchCubeRenderData.indices[i + 8] = 6 + offset;

		batchCubeRenderData.indices[i + 9] = 6 + offset;
		batchCubeRenderData.indices[i + 10] = 7 + offset;
		batchCubeRenderData.indices[i + 11] = 3 + offset;

		// left
		batchCubeRenderData.indices[i + 12] = 0 + offset;
		batchCubeRenderData.indices[i + 13] = 3 + offset;
		batchCubeRenderData.indices[i + 14] = 7 + offset;

		batchCubeRenderData.indices[i + 15] = 7 + offset;
		batchCubeRenderData.indices[i + 16] = 4 + offset;
		batchCubeRenderData.indices[i + 17] = 0 + offset;

		// right
		batchCubeRenderData.indices[i + 18] = 2 + offset;
		batchCubeRenderData.indices[i + 19] = 1 + offset;
		batchCubeRenderData.indices[i + 20] = 5 + offset;

		batchCubeRenderData.indices[i + 21] = 5 + offset;
		batchCubeRenderData.indices[i + 22] = 6 + offset;
		batchCubeRenderData.indices[i + 23] = 2 + offset;

		// bottom
		batchCubeRenderData.indices[i + 24] = 0 + offset;
		batchCubeRenderData.indices[i + 25] = 1 + offset;
		batchCubeRenderData.indices[i + 26] = 2 + offset;

		batchCubeRenderData.indices[i + 27] = 2 + offset;
		batchCubeRenderData.indices[i + 28] = 3 + offset;
		batchCubeRenderData.indices[i + 29] = 0 + offset;

		// top
		batchCubeRenderData.indices[i + 30] = 7 + offset;
		batchCubeRenderData.indices[i + 31] = 6 + offset;
		batchCubeRenderData.indices[i + 32] = 5 + offset;

		batchCubeRenderData.indices[i + 33] = 5 + offset;
		batchCubeRenderData.indices[i + 34] = 4 + offset;
		batchCubeRenderData.indices[i + 35] = 7 + offset;

		offset += InitConstants::Voxel::VerticesPerVoxel;
	}

	// Index buffer
	GLCall(glCreateBuffers(1, &batchCubeRenderData.ib));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchCubeRenderData.ib));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, InitConstants::World::MaxIndicesCount, batchCubeRenderData.indices, GL_STATIC_DRAW));
}

void BatchCubeRenderer::DeInit()
{
	delete[] batchCubeRenderData.bufferStart;
	delete[] batchCubeRenderData.indices;
}

void BatchCubeRenderer::BeginBatch()
{
	batchCubeRenderData.bufferCurr = batchCubeRenderData.bufferStart;
}

void BatchCubeRenderer::EndBatch()
{
	GLsizeiptr size = (uint8_t*)batchCubeRenderData.bufferCurr - (uint8_t*)batchCubeRenderData.bufferStart;
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, batchCubeRenderData.dvb));
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, batchCubeRenderData.bufferStart);
}

void BatchCubeRenderer::Flush()
{
	GLCall(glBindVertexArray(batchCubeRenderData.va));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchCubeRenderData.ib));
	glDrawElements(GL_TRIANGLES, batchCubeRenderData.usedIndicesCount, GL_UNSIGNED_INT, nullptr);

	batchCubeRenderData.usedIndicesCount = 0;
}

void BatchCubeRenderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
{
	if (batchCubeRenderData.usedIndicesCount >= InitConstants::World::MaxIndicesCount)
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
	batchCubeRenderData.bufferCurr->Position = glm::vec3(position.x, position.y, position.z);
	batchCubeRenderData.bufferCurr->Color = color;
	batchCubeRenderData.bufferCurr++;

	// 1
	batchCubeRenderData.bufferCurr->Position = glm::vec3(position.x + size.x, position.y, position.z);
	batchCubeRenderData.bufferCurr->Color = color;
	batchCubeRenderData.bufferCurr++;

	// 2
	batchCubeRenderData.bufferCurr->Position = glm::vec3(position.x + size.x, position.y, position.z + size.z);
	batchCubeRenderData.bufferCurr->Color = color;
	batchCubeRenderData.bufferCurr++;

	// 3
	batchCubeRenderData.bufferCurr->Position = glm::vec3(position.x, position.y, position.z + size.z);
	batchCubeRenderData.bufferCurr->Color = color;
	batchCubeRenderData.bufferCurr++;

	// 4
	batchCubeRenderData.bufferCurr->Position = glm::vec3(position.x, position.y + size.y, position.z);
	batchCubeRenderData.bufferCurr->Color = color;
	batchCubeRenderData.bufferCurr++;

	// 5
	batchCubeRenderData.bufferCurr->Position = glm::vec3(position.x + size.x, position.y + size.y, position.z);
	batchCubeRenderData.bufferCurr->Color = color;
	batchCubeRenderData.bufferCurr++;

	// 6
	batchCubeRenderData.bufferCurr->Position = glm::vec3(position.x + size.x, position.y + size.y, position.z + size.z);
	batchCubeRenderData.bufferCurr->Color = color;
	batchCubeRenderData.bufferCurr++;

	// 7
	batchCubeRenderData.bufferCurr->Position = glm::vec3(position.x, position.y + size.y, position.z + size.z);
	batchCubeRenderData.bufferCurr->Color = color;
	batchCubeRenderData.bufferCurr++;

	batchCubeRenderData.usedIndicesCount += InitConstants::Voxel::IndicesPerVoxel;
}
