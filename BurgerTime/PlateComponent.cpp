#include "MiniginPCH.h"
#include "PlateComponent.h"
#include "FoodComponent.h"
#include "Event.h"

dae::Creator<dae::Component, PlateComponent> s_TranformComponentCreate{};

PlateComponent::PlateComponent(dae::GameObject* pGameobject)
	: dae::Component(pGameobject)
	, m_pSubject{ std::make_unique<Subject>() }
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
	m_pRigidbody = m_pGameObject->GetComponent<dae::RigidbodyComponent>();
}

void PlateComponent::Update()
{
}

void PlateComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.EndObject();
}

void PlateComponent::Deserialize(dae::GameObject* pGameobject, rapidjson::Value&)
{
	m_pGameObject = pGameobject;
}

void PlateComponent::AddIngredient(FoodComponent* pFoodComponent)
{
	pFoodComponent->GetGameObject()->SetParent(m_pGameObject);
	pFoodComponent->GetGameObject()->SetTag(m_pGameObject->GetTag());

	if (pFoodComponent->IsTop())
	{
		m_BurgerMade = true;
		m_pSubject->Notify(m_pGameObject, Event::Burger_Made);
		std::cout << "Burger has been made " << std::endl;
	}

	m_pIngredients.emplace_back(pFoodComponent);
}

std::unique_ptr<Subject>& PlateComponent::GetSubject()
{
	return m_pSubject;
}
