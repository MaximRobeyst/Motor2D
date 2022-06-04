#include "LeaderboardComponent.h"

#include <GameObject.h>
#include <TextObject.h>

#include <sstream>

const dae::Creator<dae::Component, LeaderboardComponent> g_LeaderboardCreater;

LeaderboardComponent::LeaderboardComponent(dae::GameObject* pGameobject, int targetScore)
	: dae::Component(pGameobject)
	, m_PlayerScore{ targetScore }
{
}

void LeaderboardComponent::Start()
{
	int closestScore{INT_MAX};
	for (int i = 0; i < m_pGameObject->GetAmountOfChildren(); ++i)
	{
		auto textComponent = m_pGameObject->GetChildFromIndex(i)->GetComponent<dae::TextComponent>();
		auto text = textComponent->GetText();

		std::stringstream numberExtraction;
		numberExtraction << text;

		int score;
		numberExtraction >> score;

		if (score == m_PlayerScore || abs(score - m_PlayerScore) < abs(closestScore - m_PlayerScore))
			m_pTargetText = textComponent;
	}
}

void LeaderboardComponent::Update()
{
	m_pTargetText->SetText(m_CurrentName + ": " + std::to_string(m_PlayerScore));
}

void LeaderboardComponent::AddLetter(char letter)
{
	m_CurrentName.push_back(letter);
}

void LeaderboardComponent::RemoveLetter()
{
	if (m_CurrentName.size() <= 0) return;
	m_CurrentName.pop_back();
}
