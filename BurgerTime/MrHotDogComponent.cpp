#include "MiniginPCH.h"
#include "MrHotDogComponent.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "PlayerComponent.h"

EnemyComponent::EnemyComponent(dae::GameObject* pGameobject)
	: dae::Component(pGameobject)
{
	m_pAnimatorComponent = pGameobject->GetComponent<dae::AnimatorComponent>();

	m_pAnimatorComponent->SetAnimation("WalkLeft");

	auto rigidBodyComponent = pGameobject->GetComponent<dae::RigidbodyComponent>();

	std::function<void(dae::RigidbodyComponent*, dae::RigidbodyComponent*, b2Contact*)> newFunction = [](dae::RigidbodyComponent* /*pTriggeredbody*/, dae::RigidbodyComponent* otherBody, b2Contact*)
	{
		auto pOtherGO = otherBody->GetGameobject();

		if (pOtherGO->GetTag() == "Player")
		{
			auto playerComponent = pOtherGO->GetComponent<PlayerComponent>();
			playerComponent->PlayerDeath();
		}
	};

	rigidBodyComponent->SetOnEnterFunction(newFunction);
}

void EnemyComponent::Update()
{

}
