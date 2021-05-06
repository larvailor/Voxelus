#include "stdfax.h"

#include "MaterialComponent.h"

//#include "Texture.h"
//
//namespace
//{
//	const std::string pathToBaseShader = "res/shaders/Basic.shader";
//}
//
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
//MaterialComponent::MaterialComponent(unsigned int entityId)
//	: BaseComponent(entityId, ComponentType::Material)
//	, mShader(pathToBaseShader)
//{
//	// TODO : shader
//	mShader.Bind();
//	mShader.SetUniform4f("u_Color", 0.0f, 0.8f, 0.8f, 1.0f);
//	//shader.SetUniformMat4f("u_MVP", mvp); // TODO : set u_MVP uniform on each tick
//
//	Texture texture("res/textures/lavastoneBig.png");
//	texture.Bind();
//	mShader.SetUniform1i("u_Texture", 0);
//
//	mShader.Unbind();
//}
//
////-----------------------------------------------
////		Destructors
////
//
//MaterialComponent::~MaterialComponent()
//{
//}
//
////-----------------------------------------------
////		Getters
////
//
//Shader& MaterialComponent::GetShader()
//{
//	return mShader;
//}
//
//ComponentType MaterialComponent::GetStaticComponentType()
//{
//	return ComponentType::Material;
//}
