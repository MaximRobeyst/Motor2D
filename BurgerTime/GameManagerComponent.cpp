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

GameManagerComponent::GameManagerComponent(dae::GameObject* pGameobject, dae::GameObject* pPlayer, int burgerAmount)
	: dae::Component(pGameobject)
	, m_pPlayerObject{pPlayer}
	, m_AmountOfBurgers{ burgerAmount }
{
}

void GameManagerComponent::Notify(const dae::GameObject& /*gameObject*/, const Event& action)
{
	switch (action)
	{
		case Event::Burger_Made:
			++m_CurrentBurgers;
			if (m_CurrentBurgers == m_AmountOfBurgers)
			{
				std::string nextLevel = m_pLevelComponent->GetNextLevel();
				m_pLevelComponent->RemoveLevel();

				if (nextLevel != "Level-1")
				{
					m_pGameObject->GetScene()->AddGameObject(dae::GameObject::Deserialize(m_pGameObject->GetScene(), nextLevel));
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
