#include "MiniginPCH.h"
#include "LifeDisplayComponent.h"
#include "Event.h"
#include "LifeComponent.h"

LifeDisplayComponent::LifeDisplayComponent(dae::GameObject* pGameObject, int number, const std::string& extraDisplayText)
	: dae::Component{pGameObject}
	, m_ExtraDisplayText{extraDisplayText}
{
	m_pTextComponent = pGameObject->GetComponent<dae::TextComponent>();
	m_pTextComponent->SetText(extraDisplayText + std::to_string(number));

}

void LifeDisplayComponent::Notify(const dae::GameObject& gameObject, const Event& action)
{
	switch (action)
	{
	case Event::Player_Died:
		 ChangeText(gameObject.GetComponent<LifeComponent>()->GetLives());

	default:
		break;
	}
}

void LifeDisplayComponent::ChangeText(int number)
{
	m_pTextComponent->SetText(m_ExtraDisplayText + std::to_string(number));
}
