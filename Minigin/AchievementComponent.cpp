#include "MiniginPCH.h"
#include "AchievementComponent.h"
#include "Event.h"

AchievementComponent::AchievementComponent(dae::GameObject* pGameObject)
	: dae::Component(pGameObject)
	, m_pSteamAchievements{new CSteamAchievements(m_Achievements, m_AchievmentCount)}
{
}

AchievementComponent::~AchievementComponent()
{
	delete[] m_Achievements;
	delete m_pSteamAchievements;
}

void AchievementComponent::Notify(const dae::GameObject& /*gameObject*/, const Event& action)
{
	switch (action)
	{
	case Event::Game_Won:
		m_pSteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
		break;
	}
}
