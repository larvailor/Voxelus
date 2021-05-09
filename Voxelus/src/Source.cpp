#include "stdfax.h"

#include "Shader.h"
#include "Renderer.h"
#include "BatchRenderer.h"

#include "VertexArray.h"
#include "DynamicVertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

#include "ECS/components/Components.h"
#include "ECS/entities/Camera.h"
#include "ECS/entities/Voxel.h"

#include "World.h"

unsigned int Entity::NextEntityId = 1;

float Time::DeltaTime = 0.0f;
float Time::LastFrameTime = 0.0f;

float Mouse::OffsetX = 0.0f;
float Mouse::OffsetY = 0.0f;
float Mouse::LastFrameXpos = 0.0f;
float Mouse::LastFrameYpos = 0.0f;
float Mouse::Sensitivity = 0.2f;
int Mouse::Button = 0;
int Mouse::Action = 0;
int Mouse::Mods = 0;

/////////////////////////////////////////////////
// 
//		Constants
//
/////////////////////////////////////////////////

namespace Mesh
{
	namespace Plane
	{
		float Vertices[] = {
			0.0f,  0.0f,  0.0f,
			30.0f, 0.0f,  0.0f,
			30.0f, 30.0f, 0.0f,
			0.0f,  30.0f, 0.0f
		};

		unsigned int Indices[] = {
			0, 1, 2,
			2, 3, 0
		};
	}

	//namespace Cube
	//{
	//	//	    1 - - -  0
	//	//	  / .      / |
	//	//	3  -.- - 2   |
	//	//	|   6 . .|.  4
	//	//	| .`     | /
	//	//	6 - - -  7

	//	const float HalfWidth = 10.0f;
	//	unsigned int NumberOfVertices = 8;
	//	float Vertices[] = {
	//		// x, y, z
	//		 HalfWidth,  HalfWidth, -HalfWidth,
	//		-HalfWidth,  HalfWidth, -HalfWidth,
	//		 HalfWidth,  HalfWidth,  HalfWidth,
	//		-HalfWidth,  HalfWidth,  HalfWidth,
	//		 HalfWidth, -HalfWidth, -HalfWidth,
	//		-HalfWidth, -HalfWidth, -HalfWidth,
	//		-HalfWidth, -HalfWidth,  HalfWidth,
	//		 HalfWidth, -HalfWidth,  HalfWidth
	//	};

	//	unsigned int NumberOfIndices = 14;
	//	unsigned int Indices[] = {
	//		3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0
	//	};
	//};

	namespace LightSource
	{
		//	    1 - - -  0
		//	  / .      / |
		//	3  -.- - 2   |
		//	|   6 . .|.  4
		//	| .`     | /
		//	6 - - -  7

		const float HalfWidth = 2.5f;
		unsigned int NumberOfVertices = 8;
		float Vertices[] = {
			 HalfWidth,  HalfWidth, -HalfWidth,
			-HalfWidth,  HalfWidth, -HalfWidth,
			 HalfWidth,  HalfWidth,  HalfWidth,
			-HalfWidth,  HalfWidth,  HalfWidth,
			 HalfWidth, -HalfWidth, -HalfWidth,
			-HalfWidth, -HalfWidth, -HalfWidth,
			-HalfWidth, -HalfWidth,  HalfWidth,
			 HalfWidth, -HalfWidth,  HalfWidth
		};

		unsigned int NumberOfIndices = 14;
		unsigned int Indices[] = {
			3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0
		};
	}
}

/////////////////////////////////////////////////
// 
//		Globals
//
/////////////////////////////////////////////////

World mWorld(InitConstants::World::MaxSizeX, InitConstants::World::MaxSizeY, InitConstants::World::MaxSizeZ);

//-----------------------------------------------
//		Entities
//

Camera mMainCamera;

//-----------------------------------------------
//		Shaders
//

Shader mVoxelShader;
Shader mLightSourceShader;

//-----------------------------------------------
//		Light
//

Entity mLightSource;

glm::vec3 mLightColor;

//-----------------------------------------------
//		Render
//

Renderer mRenderer;

//-----------------------------------------------
//		Input
//

bool mKeys[1024];

/////////////////////////////////////////////////
// 
//		Methods
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Shaders
//

void InitVoxelShader()
{
	mVoxelShader.Initialize(InitConstants::Voxel::PathToShader);

	mVoxelShader.Bind();
	//mVoxelShader.SetUniform1f("u_HalfWidth", Mesh::Cube::HalfWidth);
	mVoxelShader.SetUniform3f("u_LightColor", mLightColor.x, mLightColor.y, mLightColor.z);
	mVoxelShader.Unbind();
}

void InitLightSourceShader()
{
	mLightSourceShader.Initialize(InitConstants::Light::PathToShader);
}

//-----------------------------------------------
//		Light
//

void InitLight()
{
	InitLightSourceShader();
	mLightColor = glm::vec3(1.0f, 0.2f, 0.2f);

	mLightSource.GetComponent<TransformComponent>()->SetPosition(glm::vec3(InitConstants::Light::PositionX, InitConstants::Light::PositionY, InitConstants::Light::PositionZ));
}


//-----------------------------------------------
//		Input
//

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		mKeys[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		mKeys[key] = false;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Mouse::OffsetX = (static_cast<float>(xpos) - Mouse::LastFrameXpos) * Mouse::Sensitivity;
	Mouse::OffsetY = (Mouse::LastFrameYpos - static_cast<float>(ypos)) * Mouse::Sensitivity;

	Mouse::LastFrameXpos = static_cast<float>(xpos);
	Mouse::LastFrameYpos = static_cast<float>(ypos);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Mouse::Button = button;
	Mouse::Action = action;
	Mouse::Mods = mods;
}

void InitMouse(GLFWwindow* window)
{
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPos(window, static_cast<double>(InitConstants::Window::Width / 2.0f), static_cast<double>(InitConstants::Window::Height / 2.0f));

	double cursorXpos, cursorYpos;
	glfwGetCursorPos(window, &cursorXpos, &cursorYpos);
	Mouse::LastFrameXpos = static_cast<float>(cursorXpos);
	Mouse::LastFrameYpos = static_cast<float>(cursorYpos);

	Mouse::OffsetX = 0.0f;
	Mouse::OffsetY = 0.0f;
}

void InitKeyboard(GLFWwindow* window)
{
	glfwSetKeyCallback(window, KeyCallback);
}

//-----------------------------------------------
//		Main
//

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(InitConstants::Window::Width, InitConstants::Window::Height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR! glewInit() failed" << std::endl;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//-----------------------------------------------
		//		Test stuff
		//
		//Entity plane1;
		//mVoxels.push_back(plane1);

		//Entity plane2;
		//std::shared_ptr<TransformComponent> transformComponent2 = plane2.GetComponent<TransformComponent>();
		//////transformComponent->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
		//////transformComponent->SetRotation(glm::vec3(89.0f, 0.0f, 0.0f));
		//transformComponent2->SetPosition(glm::vec3(-25.0f, 0.0f, 0.0f));
		//mVoxels.push_back(plane2);

		//Entity plane3;
		//std::shared_ptr<TransformComponent> transformComponent3 = plane3.GetComponent<TransformComponent>();
		////transformComponent3->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
		////transformComponent3->SetRotation(glm::vec3(0.0f, 30.0f, 0.0f));
		//transformComponent3->SetPosition(glm::vec3(-25.0f, 35.0f, 0.0f));
		//mVoxels.push_back(plane3);

		//-----------------------------------------------
		//		Initialization
		//
		
		InitMouse(window);
		InitKeyboard(window);
		InitLight();
		InitVoxelShader();
		BatchRenderer::Init();


		

			 //Light source buffers
		VertexArray LightSourceVertexArray;
		VertexBuffer LightSourceVertexBuffer(Mesh::LightSource::Vertices, Mesh::LightSource::NumberOfVertices * 3 * sizeof(float));
		VertexBufferLayout LightSourceVertexBufferLayout;
		IndexBuffer LightSourceIndexBuffer(Mesh::LightSource::Indices, Mesh::LightSource::NumberOfIndices);

		LightSourceVertexBufferLayout.Push<float>(3);
		LightSourceVertexArray.AddBuffer(LightSourceVertexBuffer, LightSourceVertexBufferLayout);

		LightSourceVertexArray.Unbind();
		LightSourceVertexBuffer.Unbind();
		LightSourceIndexBuffer.Unbind();
			
		
		//	// Voxel buffers
		//VertexArray VoxelVertexArray;
		//VertexBuffer VoxelVertexBuffer(Mesh::Cube::Vertices, Mesh::Cube::NumberOfVertices * 3 * sizeof(float));
		//VertexBufferLayout VoxelVertexBufferLayout;
		//IndexBuffer VoxelIndexBuffer(Mesh::Cube::Indices, Mesh::Cube::NumberOfIndices);

		//VoxelVertexBufferLayout.Push<float>(3);
		//VoxelVertexArray.AddBuffer(VoxelVertexBuffer, VoxelVertexBufferLayout);

		//VoxelVertexArray.Unbind();
		//VoxelVertexBuffer.Unbind();
		//VoxelIndexBuffer.Unbind();
		//	//

		//-----------------------------------------------
		//		Loop variables
		//

		
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Poll for and process events */
			GLCall(glfwPollEvents());

			//-----------------------------------------------
			//		Update
			//

				// Delta time
			float time = static_cast<float>(glfwGetTime());
			Time::DeltaTime = time - Time::LastFrameTime;
			Time::LastFrameTime = time;
			std::cout << "DeltaTime = " << Time::DeltaTime << std::endl;

				// Mouse

			if (Mouse::Button == GLFW_MOUSE_BUTTON_MIDDLE)
			{
				if (Mouse::Action == GLFW_PRESS)
				{
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}
				else if (Mouse::Action == GLFW_RELEASE)
				{
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
			}

				// Camera
			mMainCamera.OnTickUpdate();
			mMainCamera.HandleInput(mKeys);

			//-----------------------------------------------
			//		Render
			//

			mRenderer.Clear();

			BatchRenderer::BeginBatch();
			for (std::shared_ptr<Voxel> voxel : mWorld.GetFloor())
			{
				BatchRenderer::DrawCube(
					voxel->GetComponent<TransformComponent>()->GetPosition(),
					voxel->GetSize(),
					voxel->GetColor()
				);
			}
			BatchRenderer::EndBatch();

			mVoxelShader.Bind();
			glm::mat4 viewProjMat = mMainCamera.GetProjectionMatrix() * mMainCamera.GetViewMatrix();
			mVoxelShader.SetUniformMat4f("u_ViewProj", viewProjMat);
			mVoxelShader.SetUniformMat4f("u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

			BatchRenderer::Flush();
			mVoxelShader.Unbind();

				 //Draw light sources
			mLightSourceShader.Bind();

			glm::mat4 mvp = mMainCamera.GetProjectionMatrix() * mMainCamera.GetViewMatrix() * mLightSource.GetComponent<TransformComponent>()->GetTransformMat();

			mLightSourceShader.SetUniformMat4f("u_MVP", mvp);
			mLightSourceShader.SetUniform3f("u_Color", mLightColor.x, mLightColor.y, mLightColor.z);

			mRenderer.Draw(LightSourceVertexArray, LightSourceIndexBuffer);

			mLightSourceShader.Unbind();

			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));
		}
	}


	//-----------------------------------------------
	//		Deinitialize
	//

	BatchRenderer::DeInit();

	glfwTerminate();
	return 0;
}
