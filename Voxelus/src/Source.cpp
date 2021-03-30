#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Entity.h"
#include "components/TransformComponent.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#ifndef DEBUG_MODE
	#define DEBUG_MODE
#endif //DEBUG_MODE

unsigned int Entity::NextEntityId = 1;

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
		//float positions[] = {
		//	100.0f, 100.0f, 0.0f, 0.0f, 0.0f,
		//	400.0f, 100.0f, 0.0f, 1.0f, 0.0f,
		//	400.0f, 400.0f, 0.0f, 1.0f, 1.0f,
		//	100.0f, 400.0f, 0.0f, 0.0f, 1.0f 
		//};

		float positions[] = {
			0.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			30.0f, 0.0f,  0.0f, 1.0f, 0.0f,
			30.0f, 30.0f, 0.0f, 1.0f, 1.0f,
			0.0f,  30.0f, 0.0f, 0.0f, 1.0f 
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray vertexArray;

		VertexBuffer vertexBuffer(positions, 4 * 5 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		vertexArray.AddBuffer(vertexBuffer, layout);

		IndexBuffer indexBuffer(indices, 6);




		// Test stuff

		//Entity plane;
		//plane.AddComponent<TransformComponent>();
		//if (plane.HasComponent<TransformComponent>())
		//{
		//	plane.GetMutableComponent<TransformComponent>()->SetPosition(glm::vec3(123.0f, 2.0f, 4.0f));

		//	const std::shared_ptr<TransformComponent> tc1 = plane.GetComponent<TransformComponent>();
		//	auto pos = tc1->GetPosition();

		//	plane.AddComponent<TransformComponent>();
		//}

		Entity plane;
		plane.AddComponent<TransformComponent>();
		std::shared_ptr<TransformComponent> transformComponent = plane.GetComponent<TransformComponent>();
		transformComponent->SetScale(glm::vec3(2.0f, 2.0f, 1.0f));
		transformComponent->SetRotation(glm::vec3(89.0f, 0.0f, 0.0f));
		transformComponent->SetPosition(glm::vec3(-20.0f, -20.0f, 00.0f));

		const glm::vec3 cStartPosition = glm::vec3(0.0f, 0.0f, 100.0f);
		const glm::vec3 cStartLookDirection = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
		const glm::vec3 cStartUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));

		const float cStartNearPlane = 0.01f;
		const float cStartFarPlane = 500.0f;
		const float cStartFieldOfView = 45.0f;

		glm::mat4 model = transformComponent->GetTransformMat();
		glm::mat4 view = glm::lookAt(cStartPosition, cStartPosition + cStartLookDirection, cStartUp);
		glm::mat4 proj = glm::perspective(glm::radians(cStartFieldOfView), 4.0f / 3.0f, cStartNearPlane, cStartFarPlane);
		glm::mat4 mvp = proj * view * model;

		//

		//glm::mat4 projMat = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.0f, 0.8f, 0.8f, 1.0f);
		shader.SetUniformMat4f("u_MVP", mvp);

		Texture texture("res/textures/lavastoneBig.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		vertexArray.Unbind();
		vertexBuffer.Unbind();
		indexBuffer.Unbind();
		shader.Unbind();

		Renderer renderer;


		float r = 0.0f;
		float increment = 0.01f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.8f, 0.8f, 1.0f);
			renderer.Draw(vertexArray, indexBuffer, shader);

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
