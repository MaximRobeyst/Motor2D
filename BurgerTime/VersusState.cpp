#include "VersusState.h"
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
#include <Xbox360Controller.h>

void VersusState::OnEnter()
{
	auto scene = dae::Scene::Deserialize("Level1");

	auto pPeperGameObject = new dae::GameObject("MrHotDog_Player2");
	auto pPlayerTransform = new dae::TransformComponent(pPeperGameObject, glm::vec3{ 256.f, 296.f, 0 }, glm::vec3{ 2.f });
	pPeperGameObject->AddComponent(pPlayerTransform);
	pPeperGameObject->AddComponent(new dae::SpriteRendererComponent(pPeperGameObject, "BurgerTime_SpriteSheet.png"));
	pPeperGameObject->AddComponent(new dae::AnimatorComponent(pPeperGameObject, "../Data/Animations/HotdogAnimations.json"));
	//pPeperGameObject->AddComponent(new MovementComponent(pPeperGameObject, 100.f));
	auto pLifeComponent = new LifeComponent{ pPeperGameObject, 3 };
	pPeperGameObject->AddComponent(pLifeComponent);
	pPeperGameObject->AddComponent(new dae::ColliderComponent(pPeperGameObject, 15.f, 15.f, glm::vec2{ 8.0f, 8.0f }));
	auto pRigidbody = new dae::RigidbodyComponent(pPeperGameObject, b2_dynamicBody, 1.f, 0.3f);
	pPeperGameObject->AddComponent(pRigidbody);
	auto pPlayercomp = new PlayerComponent(pPeperGameObject);
	pPeperGameObject->AddComponent(pPlayercomp);

	auto controller = std::make_shared<dae::Xbox360Controller>(0);
	dae::InputManager::GetInstance().AddController(controller);

	dae::InputManager::GetInstance().AddAxis("controller_horizontal", new dae::ControllerAxis(dae::ControllerButton::DPadRight, dae::ControllerButton::DPadLeft, dae::InputManager::GetInstance().GetController(0)));
	dae::InputManager::GetInstance().AddAxis("controller_vertical", new dae::ControllerAxis(dae::ControllerButton::DPadDown, dae::ControllerButton::DPadUp, dae::InputManager::GetInstance().GetController(0)));

	pPlayercomp->SetHorizontalAxis("controller_horizontal");
	pPlayercomp->SetVerticalAxis("controller_vertical");

	std::function<void(dae::RigidbodyComponent*, dae::RigidbodyComponent*, b2Contact*)> newFunction = [](dae::RigidbodyComponent* pTriggeredbody, dae::RigidbodyComponent* otherBody, b2Contact*)
	{
		auto pOtherGO = otherBody->GetGameObject();

		if (pOtherGO->GetTag() == "Player")
		{
			auto playerComponent = pOtherGO->GetComponent<PlayerComponent>();
			playerComponent->PlayerDeath();
		}
		else if (pOtherGO->GetTag() == "Food")
		{
			//auto enemyComp = pTriggeredbody->GetGameObject()->GetComponent<EnemyComponent>();
			pTriggeredbody->GetGameObject()->GetComponent<PlayerComponent>()->PlayerDeath();
		}
	};

	pRigidbody->SetOnEnterFunction(newFunction);
	pPeperGameObject->SetTag("Enemy");
	scene->AddGameObject(pPeperGameObject);
}

void VersusState::OnExit()
{
	dae::SceneManager::GetInstance().RemoveScene(0);
}
