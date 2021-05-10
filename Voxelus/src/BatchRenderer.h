#pragma once

class BatchLineRenderer
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

	BatchLineRenderer() = delete;
	
	//-----------------------------------------------
	//		Lifetime
	//

	static void Init();
	static void DeInit();

	//-----------------------------------------------
	//		Rendering
	//

	static void BeginBatch();
	static void EndBatch();
	static void Flush();

	static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4 color);
};

class BatchCubeRenderer
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

	BatchCubeRenderer() = delete;
	
	//-----------------------------------------------
	//		Lifetime
	//

	static void Init();
	static void DeInit();

	//-----------------------------------------------
	//		Rendering
	//

	static void BeginBatch();
	static void EndBatch();
	static void Flush();

	static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
};
