#pragma once

class BatchRenderer
{
public:
	static void Init();
	static void DeInit();

	static void BeginBatch();
	static void EndBatch();
	static void Flush();

	static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
};
