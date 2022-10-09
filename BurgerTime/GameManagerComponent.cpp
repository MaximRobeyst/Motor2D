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

#include "ScoreDisplayComponent.h"
#include "MainMenuState.h"

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

	if (m_pLevelComponent == nullptr)
		m_pLevelComponent = m_pGameObject->GetScene()->FindGameobjectWithTag("Level")->GetComponent<LevelComponent>();
}

void GameManagerComponent::Update()
{
	if (!m_LoadNext) return;


	dae::Scene::Deserialize(m_NextLevel);
	m_LoadNext = false;
}

void GameManagerComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.Key("AmountOfBurgers");
	writer.Int(m_AmountOfBurgers);
	writer.EndObject();
}
void GameManagerComponent::Deserialize(dae::GameObject* pGameobject, rapidjson::Value& value)
{
	m_pGameObject = pGameobject;
	m_AmountOfBurgers = value["AmountOfBurgers"].GetInt();
}

void GameManagerComponent::Notify(const dae::GameObject& /*gameObject*/, const Event& /*action*/)
{
	//switch (action)
	//{
	//	case Event::Burger_Made:
	//		++m_CurrentBurgers;
	//		if (m_CurrentBurgers == m_AmountOfBurgers)
	//		{
	//			m_NextLevel = m_pLevelComponent->GetNextLevel();
	//			m_pLevelComponent->RemoveLevel();
	//
	//
	//			if (m_NextLevel == "Level-1")
	//			{
	//				GameStateManager::GetInstance().SwitchGameState(new LeaderboardState(ScoreDisplayComponent::GetScore()));
	//			}
	//			else if (m_NextLevel == "Main_Menu")
	//			{
	//				GameStateManager::GetInstance().SwitchGameState(new MainMenuState());
	//			}
	//			else
	//			{
	//				dae::SceneManager::GetInstance().RemoveScene(m_pGameObject->GetScene()->GetName());
	//				m_LoadNext = true;
	//			}
	//		}
	//		break;
	//}
}

void GameManagerComponent::SetLevelComponent(LevelComponent* pLevelComponent)
{
	m_pLevelComponent = pLevelComponent;
}

void GameManagerComponent::LoadNextLevel()
{
}
