#include "stdfax.h"

#include "MeshComponent.h"

///////////////////////////////////////////////////
//// 
////		PUBLIC METHODS
////
///////////////////////////////////////////////////
//
////-----------------------------------------------
////		Constructors
////
//
//MeshComponent::MeshComponent(unsigned int entityId)
//	: BaseComponent(entityId, ComponentType::Mesh)
//	, mVertexBuffer(mCubeVertices, 4 * 5 * sizeof(float))
//	, mIndexBuffer(mCubeIndices, 6)
//{
//	mVertexBufferLayout.Push<float>(3);
//	mVertexBufferLayout.Push<float>(2);
//
//	mVertexArray.AddBuffer(mVertexBuffer, mVertexBufferLayout);
//
//	mVertexArray.Unbind();
//	mVertexBuffer.Unbind();
//	mIndexBuffer.Unbind();
//}
//
////-----------------------------------------------
////		Constructors
////
//
//MeshComponent::~MeshComponent()
//{
//
//}
//
////-----------------------------------------------
////		Getters
////
//
//VertexArray& MeshComponent::GetVertexArray()
//{
//	return mVertexArray;
//}
//
//IndexBuffer& MeshComponent::GetIndexBuffer()
//{
//	return mIndexBuffer;
//}
//
//ComponentType MeshComponent::GetStaticComponentType()
//{
//	return ComponentType::Mesh;
//}
