#include "LeaderboardComponent.h"

#include <GameObject.h>
#include <TextObject.h>

#include <sstream>
#include "ScoreDisplayComponent.h"

const dae::Creator<dae::Component, LeaderboardComponent> g_LeaderboardCreater;

LeaderboardComponent::LeaderboardComponent()
	: m_PlayerScore{ScoreDisplayComponent::GetScore()}
{
}

LeaderboardComponent::LeaderboardComponent(dae::GameObject* pGameobject, int /*targetScore*/)
	: dae::Component(pGameobject)
	, m_PlayerScore{ ScoreDisplayComponent::GetScore()}
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

		int score{};
		std::string temp{};
		while (!numberExtraction.eof())
		{
			numberExtraction >> temp;

			if(std::stringstream(temp) >> score)
				break;
		}

		if (score == m_PlayerScore || abs(score - m_PlayerScore) < abs(closestScore - m_PlayerScore))
		{
			closestScore = score;
			m_pTargetText = textComponent;
		}
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
