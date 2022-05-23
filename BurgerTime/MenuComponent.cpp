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
	auto pTransform = new dae::TransformComponent(m_pMenuPointer,
		m_pButtonObjects[m_CurrentSelection]->GetGameObject()->GetComponent<dae::TransformComponent>()->GetPosition(), glm::vec2{ 2.f });
	m_pMenuPointer->AddComponent(pTransform);
	auto pSpriteRenderer = new dae::SpriteRendererComponent(m_pMenuPointer, "MainCharacter.png");
	m_pMenuPointer->AddComponent(pSpriteRenderer);
	pTransform->SetPosition(pTransform->GetPosition() - glm::vec3{ pSpriteRenderer->GetSampleRectangle().w * pTransform->GetScale().x, 0.f, 0.f });
	

	m_pGameObject->GetScene()->AddGameObject(m_pMenuPointer);
}

void MenuComponent::SwitchSelection(int i)
{
	m_CurrentSelection += i;
	if (m_CurrentSelection == -1)
		m_CurrentSelection = m_pButtonObjects.size() - 1;
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
