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
	if(pGamestate != nullptr)
		pGamestate->OnEnter();
	if(m_pCurrentState != nullptr)
		m_pCurrentState->OnExit();

	m_pCurrentState = pGamestate;
}

IGameState* GameStateManager::GetGameState()
{
	return m_pCurrentState;
}
