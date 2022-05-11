#include "MiniginPCH.h"
#include "PlateComponent.h"
#include "FoodComponent.h"

dae::Creator<dae::Component, PlateComponent> s_TranformComponentCreate{};

PlateComponent::PlateComponent(dae::GameObject* pGameobject)
	: dae::Component(pGameobject)
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

void PlateComponent::AddIngredient(dae::GameObject* pGameobject)
{
	pGameobject->SetParent(m_pGameObject);
	pGameobject->SetTag(m_pGameObject->GetTag());

	m_pIngredients.push_back(pGameobject);
}
