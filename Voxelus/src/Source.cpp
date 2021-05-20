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
#include "Ray.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

unsigned int Entity::NextEntityId = 1;

float Time::DeltaTime = 0.0f;
float Time::LastFrameTime = 0.0f;

double Mouse::OffsetX = 0.0;
double Mouse::OffsetY = 0.0;
double Mouse::LastFrameXpos = 0.0;
double Mouse::LastFrameYpos = 0.0;
double Mouse::Sensitivity = 0.2;
int Mouse::Button = 0;
int Mouse::Action = 0;
int Mouse::Mods = 0;
bool Mouse::mIsLMB_Pressed = false;
bool Mouse::mWasLMB_Pressed = false;
bool Mouse::mWasLMB_Released = false;

float Instruments::BrushColorR = 0.7f;
float Instruments::BrushColorG = 0.2f;
float Instruments::BrushColorB = 0.3f;
Instruments::Type Instruments::Current = Instruments::Type::Add;

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
			// x, y, z
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
bool IsRenderMode = false;

std::string saveName = "test1.txt";
std::string loadName = "test1.txt";

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

void SaveModel()
{
	std::ofstream saveFile;
	saveFile.open(saveName);

	if (saveFile.is_open())
	{
		for (std::shared_ptr<Voxel>& voxel : mWorld.GetVoxels())
		{
			if (!voxel->IsFloorPart)
			{
				glm::vec3 position = voxel->GetComponent<TransformComponent>()->GetPosition();
				saveFile << position.x << " " << position.y << " " << position.z << " ";
				glm::vec4 color = voxel->GetColor();
				saveFile << color.x << " " << color.y << " " << color.z << "\n";
			}
		}
	}

	saveFile.close();
}

void LoadModel()
{
	std::string line;
	std::stringstream ss;
	std::ifstream loadFile(loadName);

	glm::vec3 position;
	glm::vec4 color;
	if (loadFile.is_open())
	{
		while (getline(loadFile, line))
		{
			ss.clear();
			ss.str(line);

			ss >> position.x >> position.y >> position.z;
			ss >> color.x >> color.y >> color.z;
			color.w = 1.0f;

			mWorld.CreateVoxel(position, color);
		}
		loadFile.close();
	}
}

//-----------------------------------------------
//		Shaders
//

void InitVoxelShader()
{
	mVoxelShader.Initialize(InitConstants::Voxel::PathToShader);
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
	mLightColor = glm::vec3(InitConstants::Light::ColorR, InitConstants::Light::ColorG, InitConstants::Light::ColorB);

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
	 
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		IsRenderMode = !IsRenderMode;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Mouse::OffsetX = (xpos - Mouse::LastFrameXpos) * Mouse::Sensitivity;
	Mouse::OffsetY = (Mouse::LastFrameYpos - ypos) * Mouse::Sensitivity;

	Mouse::LastFrameXpos = xpos;
	Mouse::LastFrameYpos = ypos;
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
	Mouse::LastFrameXpos = cursorXpos;
	Mouse::LastFrameYpos = cursorYpos;

	Mouse::OffsetX = 0.0;
	Mouse::OffsetY = 0.0;
}

void InitKeyboard(GLFWwindow* window)
{
	glfwSetKeyCallback(window, KeyCallback);
}

//-----------------------------------------------
//		Else
//

Ray& CalculateRay()
{
	// Mouse coords to OpenGL coords
	double x = (2.0 * Mouse::LastFrameXpos) / InitConstants::Window::Width - 1.0;
	double y = 1.0 - (2.0 * Mouse::LastFrameYpos) / InitConstants::Window::Height;

	// Homogeneous clip coords
	glm::vec4 rayClipCoords = glm::vec4(x, y, -1.0f, 1.0f);

	// To camera coords
	glm::vec4 rayCameraCoords = glm::inverse(mMainCamera.GetProjectionMatrix()) * rayClipCoords;
	rayCameraCoords = glm::vec4(rayCameraCoords.x, rayCameraCoords.y, -1.0f, 0.0f);
		
	// To world coords
	glm::vec3 rayWorldCoords = glm::inverse(mMainCamera.GetViewMatrix()) * rayCameraCoords;

	Ray ray(mMainCamera.GetComponent<TransformComponent>()->GetPosition(), rayWorldCoords, 1000.0f);
	return ray;
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
		//		Initialization
		//
		
		InitMouse(window);
		InitKeyboard(window);
		InitLight();
		InitVoxelShader();
		BatchLineRenderer::Init();
		BatchCubeRenderer::Init();

		LoadModel();

			// Base cube buffers
		VertexArray baseCubeVA;
		VertexBuffer baseCubeVB(Mesh::LightSource::Vertices, Mesh::LightSource::NumberOfVertices * 3 * sizeof(float));
		VertexBufferLayout baseCubeVBL;
		IndexBuffer baseCubeIB(Mesh::LightSource::Indices, Mesh::LightSource::NumberOfIndices);

		baseCubeVBL.Push<float>(3);
		baseCubeVA.AddBuffer(baseCubeVB, baseCubeVBL);

		baseCubeVA.Unbind();
		baseCubeVB.Unbind();
		baseCubeIB.Unbind();

		//-----------------------------------------------
		//		Loop variables
		//

		std::vector<Ray> rays;
		Ray ray;
		bool lmbPressed = false;


		// Setup ImGui binding
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplGlfwGL3_Init(window, false);

		// Setup style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		ImVec4 BrushNewColor = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

		bool instruments = false;
		char saveModelPath[1024] = "";
		char loadModelPath[1024] = "";
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Poll for and process events */
			GLCall(glfwPollEvents());

			//-----------------------------------------------
			//		Update
			//
			
			ImGui_ImplGlfwGL3_NewFrame();

				// Delta time
			float time = static_cast<float>(glfwGetTime());
			Time::DeltaTime = time - Time::LastFrameTime;
			Time::LastFrameTime = time;
			//std::cout << "DeltaTime = " << Time::DeltaTime << std::endl;

				// Mouse
			Mouse::mWasLMB_Pressed = false;
			Mouse::mWasLMB_Released = false;
			if (Mouse::Button == GLFW_MOUSE_BUTTON_LEFT)
			{
				if (Mouse::Action == GLFW_PRESS)
				{
					if (!Mouse::mIsLMB_Pressed)
					{
						Mouse::mWasLMB_Pressed = true;
					}
					Mouse::mIsLMB_Pressed = true;
				}
				else if (Mouse::Action == GLFW_RELEASE)
				{
					Mouse::mIsLMB_Pressed = false;
					Mouse::mWasLMB_Released = true;
				}
			}

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
			
				// Instruments

			Instruments::BrushColorR = BrushNewColor.x;
			Instruments::BrushColorG = BrushNewColor.y;
			Instruments::BrushColorB = BrushNewColor.z;
		
				// Camera
			mMainCamera.OnTickUpdate();
			mMainCamera.HandleInput(mKeys);

				// Ray

			if (!IsRenderMode)
			{
				ray = CalculateRay();

				// World
				mWorld.ProcessHoveringVoxels(ray);
				mWorld.OnProcessTick(mKeys);
			}

			//-----------------------------------------------
			//		Render
			//

			mRenderer.Clear();

			// Batch
				// Floor
			BatchCubeRenderer::BeginBatch();
			for (std::shared_ptr<Voxel>& voxel : mWorld.GetVoxels())
			{
				if (!IsRenderMode)
				{
					BatchCubeRenderer::DrawCube(
						voxel->GetComponent<TransformComponent>()->GetPosition(),
						voxel->GetSize(),
						voxel->GetColor()
					);
				}
				else if (!voxel->IsFloorPart)
				{
					BatchCubeRenderer::DrawCube(
						voxel->GetComponent<TransformComponent>()->GetPosition(),
						voxel->GetSize(),
						voxel->GetColor()
					);
				}
			}
			
			if (!IsRenderMode)
			{
				for (std::shared_ptr<Voxel>& voxel : mWorld.GetPossibleVoxels())
				{
					BatchCubeRenderer::DrawCube(
						voxel->GetComponent<TransformComponent>()->GetPosition(),
						voxel->GetSize(),
						voxel->GetColor()
					);
				}
			}
			BatchCubeRenderer::EndBatch();

			mVoxelShader.Bind();
			glm::mat4 viewProjMat = mMainCamera.GetProjectionMatrix() * mMainCamera.GetViewMatrix();
			mVoxelShader.SetUniformMat4f("u_ViewProj", viewProjMat);
			mVoxelShader.SetUniformMat4f("u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

			glm::vec3 lightPos = mLightSource.GetComponent<TransformComponent>()->GetPosition();
			mVoxelShader.SetUniform3f("u_LightPos", lightPos.x, lightPos.y, lightPos.z);
			mVoxelShader.SetUniform3f("u_LightColor", mLightColor.x, mLightColor.y, mLightColor.z);

			BatchCubeRenderer::Flush();
			mVoxelShader.Unbind();

			//	// Draw lines
			//BatchLineRenderer::BeginBatch();
			//for (Ray& ray : rays)
			//{
			//	BatchLineRenderer::DrawLine(
			//		ray.GetOrigin(),
			//		ray.GetOrigin() + ray.GetDirection() * 1000.0f,
			//		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
			//	);
			//}
			//BatchLineRenderer::EndBatch();

			//mVoxelShader.Bind();
			//mVoxelShader.SetUniformMat4f("u_ViewProj", viewProjMat);
			//mVoxelShader.SetUniformMat4f("u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

			//lightPos = mLightSource.GetComponent<TransformComponent>()->GetPosition();
			//mVoxelShader.SetUniform3f("u_LightPos", lightPos.x, lightPos.y, lightPos.z);
			//mVoxelShader.SetUniform3f("u_LightColor", mLightColor.x, mLightColor.y, mLightColor.z);

			//BatchLineRenderer::Flush();
			//mVoxelShader.Unbind();

				// Draw coordinates directions
			if (!IsRenderMode)
			{
				mLightSourceShader.Bind();
				for (std::shared_ptr<Voxel> direction : mWorld.GetCoordinateDirections())
				{
					glm::mat4 mvp = mMainCamera.GetProjectionMatrix() * mMainCamera.GetViewMatrix() * direction->GetComponent<TransformComponent>()->GetTransformMat();
					glm::vec4 directionColor = direction->GetColor();

					mLightSourceShader.SetUniformMat4f("u_MVP", mvp);
					mLightSourceShader.SetUniform3f("u_Color", directionColor.x, directionColor.y, directionColor.z);

					mRenderer.Draw(baseCubeVA, baseCubeIB);
				}
				mLightSourceShader.Unbind();
			}

				// Draw light sources
			mLightSourceShader.Bind();

			glm::mat4 mvp = mMainCamera.GetProjectionMatrix() * mMainCamera.GetViewMatrix() * mLightSource.GetComponent<TransformComponent>()->GetTransformMat();

			mLightSourceShader.SetUniformMat4f("u_MVP", mvp);
			mLightSourceShader.SetUniform3f("u_Color", mLightColor.x, mLightColor.y, mLightColor.z);

			mRenderer.Draw(baseCubeVA, baseCubeIB);

			mLightSourceShader.Unbind();

			if (!IsRenderMode)
			{
				ImGui::Begin("Instruments", &instruments, ImGuiCond_Once);
				ImGui::ColorEdit3("Color", (float*)&BrushNewColor);
				if (ImGui::Button("Add", ImVec2(100.0f, 40.0f)))
				{
					Instruments::Current = Instruments::Type::Add;
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete", ImVec2(100.0f, 40.0f)))
				{
					Instruments::Current = Instruments::Type::Delete;
				}
				ImGui::SameLine();
				if (ImGui::Button("Brush", ImVec2(100.0f, 40.0f)))
				{
					Instruments::Current = Instruments::Type::Brush;
				}
				ImGui::InputText("", loadModelPath, IM_ARRAYSIZE(loadModelPath)); ImGui::SameLine();
				if (ImGui::Button("Load", ImVec2(60.0f, 20.0f)))
				{
					// load
				}

				ImGui::InputText("", loadModelPath, IM_ARRAYSIZE(loadModelPath));
				ImGui::SameLine();
				if (ImGui::Button("Save", ImVec2(60.0f, 20.0f)))
				{
					// load
				}

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));
		}
	}

	SaveModel();

	//-----------------------------------------------
	//		Deinitialize
	//

	BatchCubeRenderer::DeInit();

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}
