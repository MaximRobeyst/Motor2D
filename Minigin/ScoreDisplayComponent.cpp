#include "MiniginPCH.h"
#include "ScoreDisplayComponent.h"
#include "Observer.h"
#include "Event.h"

ScoreDisplayComponent::ScoreDisplayComponent(dae::GameObject* pGameObject, int number, const std::string& extraDisplayText)
	: Component{ pGameObject }
	, m_Score{number}
	, m_ExtraDisplayText{extraDisplayText}
	, m_pTextComponent{ pGameObject->GetComponent<dae::TextComponent>() }
{
}

void ScoreDisplayComponent::Notify(const dae::GameObject& /*gameObject*/, const Event& action)
{
	switch (action)
	{
	case Event::Enemy_Died:
	case Event::Burger_Drop:
		m_Score += 100;
		ChangeText(m_Score);
		break;

	default:
		break;
	}
}

void ScoreDisplayComponent::ChangeText(int number)
{
	m_pTextComponent->SetText(m_ExtraDisplayText + std::to_string(number));
}
