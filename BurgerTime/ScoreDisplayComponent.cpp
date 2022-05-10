#include "ScoreDisplayComponent.h"
#include "Observer.h"
#include "Event.h"
#include "Subject.h"
#include "MrHotDogComponent.h"

#include <GameObject.h>
#include <string>

ScoreDisplayComponent::ScoreDisplayComponent(dae::GameObject* pGameObject, int number, const std::string& extraDisplayText)
	: Component{ pGameObject }
	, m_Score{number}
	, m_ExtraDisplayText{extraDisplayText}
	, m_pTextComponent{ pGameObject->GetComponent<dae::TextComponent>() }
	, m_pSubject{std::make_unique<Subject>()}
{
	m_pTextComponent->SetText(extraDisplayText + std::to_string(number));
}

void ScoreDisplayComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(this).name());
	writer.EndObject();
}

void ScoreDisplayComponent::Notify(const dae::GameObject& gameObject, const Event& action)
{
	switch (action)
	{
	case Event::Enemy_Died:
	{
		auto pEnemyComponent = gameObject.GetComponent<EnemyComponent>();

		m_Score += pEnemyComponent->GetScore();
		ChangeText(m_Score);
		break;
	}
	case Event::Burger_Drop:
	{
		m_Score += 50;
		ChangeText(m_Score);
		if (m_Score == 500)
			m_pSubject->Notify(m_pGameObject, Event::Game_Won);
		break;
	}
	default:
		break;
	}
}

void ScoreDisplayComponent::ChangeText(int number)
{
	m_pTextComponent->SetText(m_ExtraDisplayText + std::to_string(number));
}

std::unique_ptr<Subject>& ScoreDisplayComponent::GetSubject()
{
	return m_pSubject;
}
