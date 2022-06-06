#include "ScoreDisplayComponent.h"
#include "Observer.h"
#include "Event.h"
#include "Subject.h"
#include "MrHotDogComponent.h"

#include <GameObject.h>
#include <string>
#include "FoodComponent.h"
#include <Factory.h>
#include <Scene.h>

#include "LifeComponent.h"

const dae::Creator<dae::Component, ScoreDisplayComponent> g_ScoredisplayCreator{};

int ScoreDisplayComponent::m_Score{};


ScoreDisplayComponent::ScoreDisplayComponent()
	: m_pSubject{std::make_unique<Subject>()}
{
}

ScoreDisplayComponent::ScoreDisplayComponent(dae::GameObject* pGameObject, int /*number*/, const std::string& extraDisplayText)
	: Component{ pGameObject }
	, m_ExtraDisplayText{extraDisplayText}
	, m_pSubject{std::make_unique<Subject>()}
{
}

void ScoreDisplayComponent::Start()
{
	SetId(m_pGameObject->GetId());
	m_pTextComponent = m_pGameObject->GetComponent<dae::TextComponent>();
	m_pTextComponent->SetText(m_ExtraDisplayText + std::to_string(m_Score));
}

void ScoreDisplayComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.Key("extraDisplayText");
	writer.String(m_ExtraDisplayText.c_str());
	writer.Key("Observers");
	writer.StartArray();
	for (int i = 0; i < m_pSubject->GetNrOfObservers(); ++i)
	{
		writer.Int(m_pSubject->GetObserverFromId(i)->GetId());
	}
	writer.EndArray();
	writer.EndObject();
}

void ScoreDisplayComponent::Deserialize(dae::GameObject* pGameobject, rapidjson::Value& value)
{
	m_pGameObject = pGameobject;
	
	m_ExtraDisplayText = value["extraDisplayText"].GetString();

	for (auto iter = value["Observers"].Begin(); iter != value["Observers"].End(); ++iter)
	{
		auto pObserver = pGameobject->GetScene()->GetGameobjectFromId(iter->GetInt());

		m_pSubject->AddObserver(pObserver->GetComponent<Observer>());

	}

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
		if (auto foodComp = gameObject.GetComponent<FoodComponent>(); foodComp->GetAmountOfEnemies() > 0)
			m_Score += foodComp->GetAmountOfEnemies() * 500;
		else
			m_Score += 50;
		ChangeText(m_Score);
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

int ScoreDisplayComponent::GetScore()
{
	return m_Score;
}

void ScoreDisplayComponent::ResetScore()
{
	m_Score = 0;
}

std::unique_ptr<Subject>& ScoreDisplayComponent::GetSubject()
{
	return m_pSubject;
}
