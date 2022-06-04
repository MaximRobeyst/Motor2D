#include "MenuComponent.h"
#include "UIButtonComponent.h"

#include <GameObject.h>
#include <Scene.h>
#include <SpriteRendererComponent.h>

dae::Creator<dae::Component, MenuComponent> g_MenuComponent{};

MenuComponent::MenuComponent(dae::GameObject* pGameobject, dae::GameObject* pPointer)
	: Component(pGameobject)
	, m_pMenuPointer{pPointer}
{
}

void MenuComponent::Start()
{
	for (int i = 0; i < m_pGameObject->GetAmountOfChildren(); ++i)
	{
		m_pButtonObjects.push_back(m_pGameObject->GetChildFromIndex(i)->GetComponent<UIButtonComponent>());
	}
}

void MenuComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.Key("pointer");
	writer.Int(m_pMenuPointer->GetId());
	writer.EndObject();
}

void MenuComponent::Deserialize(dae::GameObject* pGameobject, rapidjson::Value& value)
{
	m_pGameObject = pGameobject;
	m_pMenuPointer = pGameobject->GetScene()->GetGameobjectFromId(value["pointer"].GetInt());
}

void MenuComponent::SwitchSelection(int i)
{
	m_CurrentSelection += i;
	if (m_CurrentSelection == -1)
		m_CurrentSelection = static_cast<int>(m_pButtonObjects.size()) - 1;
	m_CurrentSelection %= m_pButtonObjects.size();

	auto pTransformComponent = m_pMenuPointer->GetComponent<dae::TransformComponent>();
	pTransformComponent->SetPosition(m_pButtonObjects[m_CurrentSelection]->GetGameObject()->GetComponent<dae::TransformComponent>()->GetPosition());
	pTransformComponent->SetPosition(pTransformComponent->GetPosition() - 
		glm::vec3{ m_pMenuPointer->GetComponent<dae::SpriteRendererComponent>()->GetSampleRectangle().w * pTransformComponent->GetScale().x, 0.f, 0.f});
}

void MenuComponent::PressButton()
{
	m_pButtonObjects[m_CurrentSelection]->CallOnClickFunction();
}
