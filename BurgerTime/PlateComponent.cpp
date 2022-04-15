#include "MiniginPCH.h"
#include "PlateComponent.h"
#include "FoodComponent.h"

PlateComponent::PlateComponent(dae::GameObject* pGameobject)
	: dae::Component(pGameobject)
	, m_pRigidbody{ pGameobject->GetComponent<dae::RigidbodyComponent>() }
{
	//std::function<void(dae::RigidbodyComponent*, dae::RigidbodyComponent*, b2Contact*)> newFunction = [](dae::RigidbodyComponent* pTriggeredBody, dae::RigidbodyComponent* otherBody, b2Contact*)
	//{
	//	auto pOtherGO = otherBody->GetGameobject();
	//
	//	if (pOtherGO->GetTag() == "Food")
	//	{
	//		pTriggeredBody->GetGameobject()->GetComponent<PlateComponent>()->AddIngredient(pOtherGO);
	//	}
	//};
	//m_pRigidbody->SetOnEnterFunction(newFunction);
}

void PlateComponent::Start()
{
}

void PlateComponent::Update()
{
}

void PlateComponent::AddIngredient(dae::GameObject* pGameobject)
{
	pGameobject->SetParent(m_pGameObject);
	pGameobject->SetTag(m_pGameObject->GetTag());

	m_pIngredients.push_back(pGameobject);
}
