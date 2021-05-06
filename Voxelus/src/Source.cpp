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
		const float cStartFarPlane = 500.0f;
		const float cStartFieldOfView = 60.0f;

		const ProjectionType cStartProjectionType = ProjectionType::Perspective;
	}

	namespace Shaders
	{
		const std::string VoxelPath = "res/shaders/Voxel.shader";
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

		 //	    0 - - -  1
		 //	  / .      / |
		 //	4  -.- - 5   |
		 //	|   3 . .|.  2
		 //	| .`     | /
		 //	7 - - -  6


		const float x = 10.0f;
		const float y = 10.0f;
		const float z = 10.0f;
		unsigned int NumberOfVertices = 8;
		float Vertices[] = {
			-x,  y, -z,
			 x,  y, -z,
			 x, -y, -z,
			-x, -y, -z,
			-x,  y,  z,
			 x,  y,  z,
			 x, -y,  z,
			-x, -y,  z,
		};
	
		unsigned int NumberOfIndices = 36;
		unsigned int Indices[] = {
			0, 1, 2,
			0, 2, 3,
			2, 1, 5,
			2, 5, 6,
			3, 2, 6,
			3, 6, 7,
			0, 3, 7,
			0, 7, 4,
			1, 0, 4,
			1, 4, 5,
			6, 5, 4,
			6, 4, 7,
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
//		Shaders
//

Shader mVoxelShader;

//-----------------------------------------------
//		Render
//

Renderer mRenderer;



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

//-----------------------------------------------
//		Shaders
//

void InitVoxelShader()
{
	mVoxelShader.Initialize(InitConstants::Shaders::VoxelPath);
	mVoxelShader.Unbind();
}

//-----------------------------------------------
//		Voxels
//


//-----------------------------------------------
//		Floor
//

//-----------------------------------------------
//		Else
//


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

	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR! glewInit() failed" << std::endl;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		float r = 0.0f;
		float increment = 0.01f;


		//-----------------------------------------------
		//		Test stuff
		//
		Entity plane1;
		mVoxels.push_back(plane1);

		//Entity plane2;
		//std::shared_ptr<TransformComponent> transformComponent2 = plane2.GetComponent<TransformComponent>();
		//////transformComponent->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
		//////transformComponent->SetRotation(glm::vec3(89.0f, 0.0f, 0.0f));
		//transformComponent2->SetPosition(glm::vec3(-75.0f, -50.0f, 0.0f));

		//mVoxels.push_back(plane2);

		//-----------------------------------------------
		//		Initialization
		//

		InitCamera();

		InitVoxelShader();

			// Voxel buffers
		VertexArray VoxelVertexArray;
		VertexBuffer VoxelVertexBuffer(Mesh::Cube::Vertices, Mesh::Cube::NumberOfVertices * 3 * sizeof(float));
		VertexBufferLayout VoxelVertexBufferLayout;
		IndexBuffer VoxelIndexBuffer(Mesh::Cube::Indices, Mesh::Cube::NumberOfIndices);

		VoxelVertexBufferLayout.Push<float>(3);
		VoxelVertexArray.AddBuffer(VoxelVertexBuffer, VoxelVertexBufferLayout);

		VoxelVertexArray.Unbind();
		VoxelVertexBuffer.Unbind();
		VoxelIndexBuffer.Unbind();
			//


		glm::mat4 viewMat;
		glm::mat4 projMat;
		glm::mat4 mvp;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			mRenderer.Clear();

			// Math
			viewMat = GetViewMat();
			projMat = GetProjectionMat();

			// Test draw one entity
			mVoxelShader.Bind();

			for (Entity& voxel : mVoxels)
			{
				mvp = projMat * viewMat * voxel.GetComponent<TransformComponent>()->GetTransformMat();

				mVoxelShader.SetUniformMat4f("u_MVP", mvp);
				mVoxelShader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

				mRenderer.Draw(VoxelVertexArray, VoxelIndexBuffer);
			}

			if (r > 1.0f || r < 0.0f)
			{
				increment *= -1;
			}
			r += increment;

			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			GLCall(glfwPollEvents());
		}
	}

	glfwTerminate();
	return 0;
}
