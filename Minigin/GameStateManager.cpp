#include "MiniginPCH.h"
#include "GameStateManager.h"

GameStateManager::GameStateManager(IGameState* pStartState)
	: m_pCurrentState{pStartState}
{
	m_pCurrentState->OnEnter();
}

GameStateManager::~GameStateManager()
{
	if(m_pCurrentState)
		delete m_pCurrentState;
}

void GameStateManager::Notify(const dae::GameObject& /*gameObject*/, const Event& /*action*/)
{
}
