#include "stdfax.h"

#include "Entity.h"

#include "ECS/components/Components.h"
#include "Renderer.h"

/////////////////////////////////////////////////
// 
//		PUBLIC METHODS
//
/////////////////////////////////////////////////

//-----------------------------------------------
//		Constructors
//

Entity::Entity()
{
	mId = NextEntityId;
	NextEntityId += 1;

	AddComponent<TransformComponent>();
}

//-----------------------------------------------
//		Destructors
//

Entity::~Entity()
{
	
}

//-----------------------------------------------
//		Render
//

//void Entity::Draw(const Renderer& renderer, glm::mat4& viewMat, glm::mat4& projMat)
//{
//	//if (HasComponent<MeshComponent>() && HasComponent<MaterialComponent>())
//	//{
//	//	glm::mat4 mvpMat = projMat * viewMat * GetComponent<TransformComponent>()->GetTransformMat();
//	//	
//	//	std::shared_ptr<MaterialComponent> materialComponent = GetComponent<MaterialComponent>();
//	//	Shader& shader = materialComponent->GetShader();
//	//	//shader.Bind();
//	//	//shader.SetUniformMat4f("u_MVP", mvpMat);
//	//	//shader.Unbind();
//
//	//	std::shared_ptr<MeshComponent> meshComponent = GetComponent<MeshComponent>();
//
//	//	renderer.Draw(meshComponent->GetVertexArray(), meshComponent->GetIndexBuffer(), materialComponent->GetShader());
//	//}
//}

//-----------------------------------------------
//		Getters
//

unsigned int Entity::GetId() const
{
	return mId;
}
