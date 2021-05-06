#include "stdfax.h"

#include "Scene.h"

//#include "ECS/components/Components.h"
//#include "Renderer.h"
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
//Scene::Scene()
//{
//	mMainCamera = std::make_shared<Entity>();
//	mMainCamera->AddComponent<CameraComponent>();
//	mMainCamera->GetComponent<TransformComponent>()->SetPosition(glm::vec3(0.0f, 0.0f, 100.0f));
//}
//
////-----------------------------------------------
////		Destructors
////
//
//Scene::~Scene()
//{
//}
//
////-----------------------------------------------
////		Render
////
//
//void Scene::Render()
//{
//	mRenderer.Clear();
//
//	std::shared_ptr<TransformComponent> mainCameraTransform = mMainCamera->GetComponent<TransformComponent>();
//	std::shared_ptr<CameraComponent> mainCamera = mMainCamera->GetComponent<CameraComponent>();
//
//	glm::mat4 viewMat = glm::lookAt(mainCameraTransform->GetPosition(), mainCameraTransform->GetPosition() + mainCamera->GetLookDirection(), mainCamera->GetUp());
//	glm::mat4 projMat = mMainCamera->GetComponent<CameraComponent>()->GetProjectionMat();
//
//	for (auto& entity : mEntities)
//	{
//		entity.get()->Draw(mRenderer, viewMat, projMat);
//	}
//}
//
////-----------------------------------------------
////		Else
////
//
//void Scene::AddEntity(std::shared_ptr<Entity>& entity)
//{
//	mEntities.push_back(entity);
//}
//
////void Scene::AddMainCamera(std::shared_ptr<Entity>& mainCamera)
////{
////	mMainCamera = mainCamera;
////}
