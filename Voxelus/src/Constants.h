#pragma once

#include <string>

namespace InitConstants
{
	namespace Voxel
	{
		const std::string PathToShader = "res/shaders/Voxel.shader";

		const unsigned int VerticesPerVoxel = 8;
		const unsigned int IndicesPerVoxel = 36;

		const float ColorR = 0.2f;
		const float ColorG = 0.7f;
		const float ColorB = 0.3f;

		const float WidthX = 20.0f;
		const float WidthY = 20.0f;
		const float WidthZ = 20.0f;
	}

	namespace World
	{
		const unsigned int MaxSizeX = 32;
		const unsigned int MaxSizeY = 32;
		const unsigned int MaxSizeZ = 32;

		constexpr unsigned int MaxVoxelCount = MaxSizeX * (MaxSizeY + 1) * MaxSizeZ;
		constexpr unsigned int MaxVerticesCount = MaxVoxelCount * Voxel::VerticesPerVoxel;
		constexpr unsigned int MaxIndicesCount = MaxVoxelCount * Voxel::IndicesPerVoxel;
	}

	namespace Floor
	{
		const std::string PathToShader = "res/shaders/Floor.shader";
	}

	namespace Window
	{
		const int Width = 1280;
		const int Height = 720;
	}

	namespace Light
	{
		const std::string PathToShader = "res/shaders/LightSource.shader";

		const float PositionX = 150.0f;
		const float PositionY = 50.0f;
		const float PositionZ = 200.0f;

		const float ColorR = 0.2f;
		const float ColorG = 0.7f;
		const float ColorB = 0.3f;
	}
}
