#include "MenuComponent.h"
#include "UIButtonComponent.h"

#include <GameObject.h>
#include <Scene.h>
#include <SpriteRendererComponent.h>

dae::Creator<dae::Component, MenuComponent> s_TranformComponentCreate{};

MenuComponent::MenuComponent(dae::GameObject* pGameobject)
	: Component(pGameobject)
{
}

void MenuComponent::Start()
{
	for (int i = 0; i < m_pGameObject->GetAmountOfChildren(); ++i)
	{
		m_pButtonObjects.push_back(m_pGameObject->GetChildFromIndex(i)->GetComponent<UIButtonComponent>());
	}

	m_pMenuPointer = new dae::GameObject("Pointer");
	m_pMenuPointer->SetParent(m_pGameObject);
	m_pMenuPointer->AddComponent(
		new dae::TransformComponent(m_pMenuPointer,
			m_pButtonObjects[m_CurrentSelection]->GetGameObject()->GetComponent<dae::TransformComponent>()->GetPosition()));
	m_pMenuPointer->AddComponent(
		new dae::SpriteRendererComponent(m_pMenuPointer, "MainCharacter.png"));
	

	m_pGameObject->GetScene()->AddGameObject(m_pMenuPointer);
}

void MenuComponent::SwitchSelection(int i)
{
	m_CurrentSelection += i;
	m_CurrentSelection %= m_pButtonObjects.size();

	m_pMenuPointer->GetComponent<dae::TransformComponent>()->SetPosition(m_pButtonObjects[m_CurrentSelection]->GetGameObject()->GetComponent<dae::TransformComponent>()->GetPosition());
}

void MenuComponent::PressButton()
{
	m_pButtonObjects[m_CurrentSelection]->CallOnClickFunction();
}
