#include "MultiplayerState.h"
#include "SceneManager.h"
#include "Scene.h"

#include <GameObject.h>
#include <Transform.h>
#include <SpriteRendererComponent.h>
#include <AnimatorComponent.h>
#include "LifeComponent.h"
#include <Collider.h>
#include <RigidbodyComponent.h>
#include "PlayerComponent.h"
#include <SceneManager.h>

void MultiplayerState::OnEnter()
{
	auto scene = dae::Scene::Deserialize("../Data./Scenes/Level01.json");

	auto pPeperGameObject = new dae::GameObject("PeterPete_Player2");
	auto pPlayerTransform = new dae::TransformComponent(pPeperGameObject, glm::vec3{ 224.f, 296.f, 0 }, glm::vec3{ 2.f });
	pPeperGameObject->AddComponent(pPlayerTransform);
	pPeperGameObject->AddComponent(new dae::SpriteRendererComponent(pPeperGameObject, "BurgerTime_SpriteSheet.png"));
	pPeperGameObject->AddComponent(new dae::AnimatorComponent(pPeperGameObject, "../Data/Animations/PlayerAnimations.json"));
	//pPeperGameObject->AddComponent(new MovementComponent(pPeperGameObject, 100.f));
	auto pLifeComponent = new LifeComponent{ pPeperGameObject, 3 };
	pPeperGameObject->AddComponent(pLifeComponent);
	pPeperGameObject->AddComponent(new dae::ColliderComponent(pPeperGameObject, 15.f, 15.f, glm::vec2{ 8.0f, 8.0f }));
	pPeperGameObject->AddComponent(new dae::RigidbodyComponent(pPeperGameObject, b2_dynamicBody, 1.f, 0.3f));
	pPeperGameObject->AddComponent(new PlayerComponent(pPeperGameObject));
	pPeperGameObject->SetTag("Player");
	scene->AddGameObject(pPeperGameObject);
}

void MultiplayerState::OnExit()
{
	dae::SceneManager::GetInstance().RemoveScene(0);
}
