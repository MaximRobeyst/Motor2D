#pragma once
#include "IGameState.h"
#include "Observer.h"
#include "Singleton.h"

class GameStateManager : public dae::Singleton<GameStateManager>
{
public:
	//GameStateManager(IGameState* pStartState);
	~GameStateManager();

	GameStateManager(const GameStateManager& other) = delete;
	GameStateManager(GameStateManager&& other) = delete;
	GameStateManager& operator=(const GameStateManager& other) = delete;
	GameStateManager& operator=(GameStateManager&& other) = delete;

	void SwitchGameState(IGameState* pGamestate);
	IGameState* GetGameState();

private:
	friend class Singleton<GameStateManager>;
	GameStateManager() = default;
	IGameState* m_pCurrentState;
};

