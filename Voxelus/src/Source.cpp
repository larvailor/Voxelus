#include "stdfax.h"

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "ECS/components/Components.h"
#include "ECS/entities/Entity.h"
#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

unsigned int Entity::NextEntityId = 1;

/////////////////////////////////////////////////
// 
//		Constants
//
/////////////////////////////////////////////////

namespace InitConstants
{
	namespace Camera
	{
		const glm::vec3 cStartPosition = glm::vec3(0.0f, 0.0f, 100.0f);
		const glm::vec3 cStartLookDirection = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
		const glm::vec3 cStartUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));

		const float cStartNearPlane = 0.01f;
		const float cStartFarPlane = 1000.0f;
		const float cStartFieldOfView = 60.0f;

		const ProjectionType cStartProjectionType = ProjectionType::Perspective;
	}

	namespace Light
	{
		const std::string PathToShader = "res/shaders/LightSource.shader";

		const glm::vec3 Position = glm::vec3(10.0f, 25.0f, 0.0f);
		const glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	namespace Voxel
	{
		const std::string PathToShader = "res/shaders/Voxel.shader";

		const glm::vec3 Color = glm::vec3(1.0f, 0.4f, 0.4f);
	}
}

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
	};

	namespace Cube
	{
		//	    1 - - -  0
		//	  / .      / |
		//	3  -.- - 2   |
		//	|   6 . .|.  4
		//	| .`     | /
		//	6 - - -  7

		const float HalfWidth = 10.0f;
		unsigned int NumberOfVertices = 8;
		float Vertices[] = {
			 HalfWidth,  HalfWidth, -HalfWidth,	1.0f, 0.0f, 0.0f,
			-HalfWidth,  HalfWidth, -HalfWidth,	0.0f, 1.0f, 0.0f,
			 HalfWidth,  HalfWidth,  HalfWidth,	0.0f, 0.0f, 1.0f,
			-HalfWidth,  HalfWidth,  HalfWidth,	1.0f, 0.0f, 0.0f,
			 HalfWidth, -HalfWidth, -HalfWidth,	0.0f, 1.0f, 0.0f,
			-HalfWidth, -HalfWidth, -HalfWidth,	1.0f, 0.0f, 0.0f,
			-HalfWidth, -HalfWidth,  HalfWidth,	0.0f, 1.0f, 0.0f,
			 HalfWidth, -HalfWidth,  HalfWidth,	0.0f, 0.0f, 1.0f
		};

		unsigned int NumberOfIndices = 14;
		unsigned int Indices[] = {
			3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0
		};
	};

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
	};

}



/////////////////////////////////////////////////
// 
//		Globals
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Entities
//

Entity mMainCamera;

std::vector<Entity> mVoxels;
std::vector<Entity> mFloor;

//-----------------------------------------------
//		Light
//

Entity mLightSource;

Shader mLightSourceShader;

glm::vec3 mLightColor;

//-----------------------------------------------
//		Voxel
//

Shader mVoxelShader;

glm::vec3 mVoxelColor;

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
//		Camera
//

void InitCamera()
{
	mMainCamera.AddComponent<CameraComponent>();
	
	std::shared_ptr<CameraComponent> cameraComp = mMainCamera.GetComponent<CameraComponent>();
	cameraComp->SetNearPlane(InitConstants::Camera::cStartNearPlane);
	cameraComp->SetFarPlane(InitConstants::Camera::cStartFarPlane);
	cameraComp->SetFieldOfView(InitConstants::Camera::cStartFieldOfView);

	cameraComp->SetProjectionType(InitConstants::Camera::cStartProjectionType);

	cameraComp->SetLookDirection(InitConstants::Camera::cStartLookDirection);
	cameraComp->SetUp(InitConstants::Camera::cStartUp);

	mMainCamera.GetComponent<TransformComponent>()->SetPosition(InitConstants::Camera::cStartPosition);
}

glm::mat4& GetViewMat()
{
	glm::mat4 viewMat;

	std::shared_ptr<TransformComponent> cameraTransformComp = mMainCamera.GetComponent<TransformComponent>();
	std::shared_ptr<CameraComponent> cameraCameraComp = mMainCamera.GetComponent<CameraComponent>();

	viewMat = glm::lookAt(cameraTransformComp->GetPosition(), cameraTransformComp->GetPosition() + cameraCameraComp->GetLookDirection(), cameraCameraComp->GetUp());
	return viewMat;

}

glm::mat4& GetProjectionMat()
{
	glm::mat4 projMat = mMainCamera.GetComponent<CameraComponent>()->GetProjectionMat();
	return projMat;
}

void HandleCameraInput()
{
	float cameraSpeed = 1.5f;
	if (mKeys[GLFW_KEY_LEFT_SHIFT])
		cameraSpeed *= 2.0f;

	std::shared_ptr<TransformComponent> cameraTransformComp = mMainCamera.GetComponent<TransformComponent>();
	std::shared_ptr<CameraComponent> cameraCameraComp = mMainCamera.GetComponent<CameraComponent>();
	if (mKeys[GLFW_KEY_W])
		cameraTransformComp->SetPosition(cameraTransformComp->GetPosition() + cameraSpeed * cameraCameraComp->GetLookDirection());
	if (mKeys[GLFW_KEY_S])
		cameraTransformComp->SetPosition(cameraTransformComp->GetPosition() - cameraSpeed * cameraCameraComp->GetLookDirection());
	if (mKeys[GLFW_KEY_A])
		cameraTransformComp->SetPosition(cameraTransformComp->GetPosition() - glm::normalize(glm::cross(cameraCameraComp->GetLookDirection(), cameraCameraComp->GetUp())) * cameraSpeed);
	if (mKeys[GLFW_KEY_D])
		cameraTransformComp->SetPosition(cameraTransformComp->GetPosition() + glm::normalize(glm::cross(cameraCameraComp->GetLookDirection(), cameraCameraComp->GetUp())) * cameraSpeed);

	if (mKeys[GLFW_KEY_Z])
		cameraTransformComp->SetPosition(cameraTransformComp->GetPosition() - cameraSpeed * cameraCameraComp->GetUp());
	if (mKeys[GLFW_KEY_X])
		cameraTransformComp->SetPosition(cameraTransformComp->GetPosition() + cameraSpeed * cameraCameraComp->GetUp());

	//float rotationSpeed = 1.0f; // deg
	//if (mKeys[GLFW_KEY_Q])
	//	cameraTransformComp->SetPosition(cameraTransformComp->GetPosition() - cameraSpeed * cameraCameraComp->GetUp());
	//if (mKeys[GLFW_KEY_E])
	//	cameraTransformComp->SetPosition(cameraTransformComp->GetPosition() + cameraSpeed * cameraCameraComp->GetUp());
}

//-----------------------------------------------
//		Light
//

void InitLightSourceShader()
{
	mLightSourceShader.Initialize(InitConstants::Light::PathToShader);
}

void InitLight()
{
	InitLightSourceShader();
	mLightColor = InitConstants::Light::Color;

	mLightSource.GetComponent<TransformComponent>()->SetPosition(InitConstants::Light::Position);
}

//-----------------------------------------------
//		Voxels
//

void InitVoxelShader()
{
	mVoxelShader.Initialize(InitConstants::Voxel::PathToShader);

	mVoxelShader.Bind();
	mVoxelShader.SetUniform1f("u_HalfWidth", Mesh::Cube::HalfWidth);
	mVoxelShader.SetUniform3f("u_LightColor", mLightColor.x, mLightColor.y, mLightColor.z);
	mVoxelShader.Unbind();
}

void InitVoxel()
{
	InitVoxelShader();
	mVoxelColor = InitConstants::Voxel::Color;
}

//-----------------------------------------------
//		Floor
//

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

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);

	glfwSetKeyCallback(window, KeyCallback);

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
		Entity plane1;
		mVoxels.push_back(plane1);

		Entity plane2;
		std::shared_ptr<TransformComponent> transformComponent2 = plane2.GetComponent<TransformComponent>();
		////transformComponent->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
		////transformComponent->SetRotation(glm::vec3(89.0f, 0.0f, 0.0f));
		transformComponent2->SetPosition(glm::vec3(-25.0f, 0.0f, 0.0f));
		mVoxels.push_back(plane2);

		Entity plane3;
		std::shared_ptr<TransformComponent> transformComponent3 = plane3.GetComponent<TransformComponent>();
		////transformComponent->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
		////transformComponent->SetRotation(glm::vec3(89.0f, 0.0f, 0.0f));
		transformComponent3->SetPosition(glm::vec3(-25.0f, 35.0f, 0.0f));
		mVoxels.push_back(plane3);

		//-----------------------------------------------
		//		Initialization
		//

		InitCamera();
		InitLight();
		InitVoxel();

			// Light source buffers
		VertexArray LightSourceVertexArray;
		VertexBuffer LightSourceVertexBuffer(Mesh::LightSource::Vertices, Mesh::LightSource::NumberOfVertices * 3 * sizeof(float));
		VertexBufferLayout LightSourceVertexBufferLayout;
		IndexBuffer LightSourceIndexBuffer(Mesh::LightSource::Indices, Mesh::LightSource::NumberOfIndices);

		LightSourceVertexBufferLayout.Push<float>(3);
		LightSourceVertexArray.AddBuffer(LightSourceVertexBuffer, LightSourceVertexBufferLayout);

		LightSourceVertexArray.Unbind();
		LightSourceVertexBuffer.Unbind();
		LightSourceIndexBuffer.Unbind();
			//
		
			// Voxel buffers
		VertexArray VoxelVertexArray;
		VertexBuffer VoxelVertexBuffer(Mesh::Cube::Vertices, Mesh::Cube::NumberOfVertices * 6 * sizeof(float));
		VertexBufferLayout VoxelVertexBufferLayout;
		IndexBuffer VoxelIndexBuffer(Mesh::Cube::Indices, Mesh::Cube::NumberOfIndices);

		VoxelVertexBufferLayout.Push<float>(3);
		VoxelVertexBufferLayout.Push<float>(3);
		VoxelVertexArray.AddBuffer(VoxelVertexBuffer, VoxelVertexBufferLayout);

		VoxelVertexArray.Unbind();
		VoxelVertexBuffer.Unbind();
		VoxelIndexBuffer.Unbind();
			//


		//-----------------------------------------------
		//		Loop variables
		//
		glm::mat4 viewMat;
		glm::mat4 projMat;
		glm::mat4 mvp;


		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Poll for and process events */
			GLCall(glfwPollEvents());

			//-----------------------------------------------
			//		Update
			//

			HandleCameraInput();



			//-----------------------------------------------
			//		Render
			//

			mRenderer.Clear();

			// Math
			viewMat = GetViewMat();
			projMat = GetProjectionMat();
			
			// Draw light sources
			mLightSourceShader.Bind();

			mvp = projMat * viewMat * mLightSource.GetComponent<TransformComponent>()->GetTransformMat();

			mLightSourceShader.SetUniformMat4f("u_MVP", mvp);
			mLightSourceShader.SetUniform3f("u_Color", mLightColor.x, mLightColor.y, mLightColor.z);

			mRenderer.Draw(LightSourceVertexArray, LightSourceIndexBuffer);

			mLightSourceShader.Unbind();

				// Draw voxels
			mVoxelShader.Bind();

			for (Entity& voxel : mVoxels)
			{
				mvp = projMat * viewMat * voxel.GetComponent<TransformComponent>()->GetTransformMat();
				glm::vec3 voxelPosition = voxel.GetComponent<TransformComponent>()->GetPosition();

				mVoxelShader.SetUniformMat4f("u_MVP", mvp);
				mVoxelShader.SetUniform3f("u_Center", voxelPosition.x, voxelPosition.y, voxelPosition.z);
				mVoxelShader.SetUniform3f("u_Color", mVoxelColor.x, mVoxelColor.y, mVoxelColor.z);

				mRenderer.Draw(VoxelVertexArray, VoxelIndexBuffer);
			}

			mVoxelShader.Unbind();

			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));
		}
	}

	glfwTerminate();
	return 0;
}
