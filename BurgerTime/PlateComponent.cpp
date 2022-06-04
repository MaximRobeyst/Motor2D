#include "MiniginPCH.h"
#include "PlateComponent.h"
#include "FoodComponent.h"
#include "Event.h"

dae::Creator<dae::Component, PlateComponent> s_TranformComponentCreate{};

PlateComponent::PlateComponent()
	: m_pSubject{std::make_unique<Subject>() }
{
}

PlateComponent::PlateComponent(dae::GameObject* pGameobject)
	: dae::Component(pGameobject)
	, m_pSubject{ std::make_unique<Subject>() }
{
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
