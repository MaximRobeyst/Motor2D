#include "WeaponComponent.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Collider.h"
#include "PlayerComponent.h"
#include "InteractComponent.h"
#include "Transform.h"

WeaponComponent::WeaponComponent()
{
}

void WeaponComponent::Start()
{
	m_pRigidbodyComponent = m_pGameObject->GetComponent<dae::RigidbodyComponent>();
	m_pTransformComponent = m_pGameObject->GetTransform();

	std::function<void(dae::RigidbodyComponent*, dae::RigidbodyComponent*, b2Contact*)> enterFunction = [this](dae::RigidbodyComponent* , dae::RigidbodyComponent* pOtherBody, b2Contact*)
	{
		if (m_pOwner != nullptr) return;

		auto pInteractComponent = pOtherBody->GetGameObject()->GetComponent<InteractComponent>();
		if (pInteractComponent != nullptr)
		{
			pInteractComponent->SetCurrentInteractible(this);
		}
	};

	std::function<void(dae::RigidbodyComponent*, dae::RigidbodyComponent*, b2Contact*)> exitFunction = [this](dae::RigidbodyComponent* , dae::RigidbodyComponent* pOtherBody, b2Contact*)
	{
		if (m_pOwner != nullptr) return;

		auto pInteractComponent = pOtherBody->GetGameObject()->GetComponent<InteractComponent>();
		if (pOtherBody->GetGameObject()->GetTag() == "Player")
		{
			pInteractComponent->SetCurrentInteractible(nullptr);
		}
	};

	m_pRigidbodyComponent->SetOnEnterFunction(enterFunction);
	m_pRigidbodyComponent->SetOnExitFunction(exitFunction);
}

void WeaponComponent::Update()
{

}

void WeaponComponent::Interact(InteractComponent* m_pCaller)
{
	if (m_pOwner != nullptr)
	{
		// Throw away
		m_pOwner->SetCurrentInteractible(nullptr);
		m_pOwner = nullptr;
		m_pGameObject->SetParent(nullptr);
		
		m_pTransformComponent->SetPosition(glm::vec3{});
		m_pTransformComponent->SetRotation(0);
		m_pTransformComponent->SetScale(glm::vec2{});


		return;
	}

	// Equip
	m_pOwner = m_pCaller;
	m_pGameObject->SetParent(m_pCaller->GetGameObject());

}

void WeaponComponent::Attack(InteractComponent* /*m_pCaller*/)
{

}