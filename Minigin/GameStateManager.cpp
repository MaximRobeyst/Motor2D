#include "MiniginPCH.h"
#include "GameStateManager.h"

//GameStateManager::GameStateManager(IGameState* pStartState)
//	: m_pCurrentState{pStartState}
//{
//	m_pCurrentState->OnEnter();
//}

GameStateManager::~GameStateManager()
{
	if(m_pCurrentState)
		delete m_pCurrentState;
}

void GameStateManager::SwitchGameState(IGameState* pGamestate)
{
	m_pNewState = pGamestate;
}

void GameStateManager::Update()
{
	if (m_pNewState != nullptr)
	{
		if (m_pCurrentState != nullptr)
			m_pCurrentState->OnExit();
		if (m_pNewState != nullptr)
			m_pNewState->OnEnter();

		delete m_pCurrentState;
		m_pCurrentState = m_pNewState;
		m_pNewState = nullptr;
	}
}

IGameState* GameStateManager::GetGameState()
{
	return m_pCurrentState;
}
