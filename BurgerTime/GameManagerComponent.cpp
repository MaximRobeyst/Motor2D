#include "GameManagerComponent.h"
#include <GameObject.h>
#include <Subject.h>
#include "Event.h"
#include "PlayerComponent.h"
#include "LevelComponent.h"
#include <Scene.h>
#include <GameStateManager.h>
#include "LeaderboardState.h"
#include <string>
#include <Factory.h>

const dae::Creator<dae::Component, GameManagerComponent> g_GamemanagerCoponentFactor{};

GameManagerComponent::GameManagerComponent(dae::GameObject* pGameobject, dae::GameObject* pPlayer, int burgerAmount)
	: dae::Component(pGameobject)
	, m_pPlayerObject{pPlayer}
	, m_AmountOfBurgers{ burgerAmount }
{
}

void GameManagerComponent::Start()
{
	SetId(m_pGameObject->GetId());
}

void GameManagerComponent::Update()
{
	if (!m_LoadNext) return;

	m_pGameObject->GetScene()->AddGameObject(dae::GameObject::Deserialize(m_pGameObject->GetScene(), m_NextLevel));
	m_LoadNext = false;
}

void GameManagerComponent::Notify(const dae::GameObject& /*gameObject*/, const Event& action)
{
	switch (action)
	{
		case Event::Burger_Made:
			++m_CurrentBurgers;
			if (m_CurrentBurgers == m_AmountOfBurgers)
			{
				m_NextLevel = m_pLevelComponent->GetNextLevel();
				m_pLevelComponent->RemoveLevel();


				if (m_NextLevel != "Level-1")
				{
					m_LoadNext = true;
				}
				else
				{
					GameStateManager::GetInstance().SwitchGameState(new LeaderboardState(100));
				}
			}
			break;
	}
}

void GameManagerComponent::SetLevelComponent(LevelComponent* pLevelComponent)
{
	m_pLevelComponent = pLevelComponent;
}

void GameManagerComponent::LoadNextLevel()
{
}
