#pragma once
#include "IGameState.h"
#include "Observer.h"

class GameStateManager : public Observer
{
public:
	GameStateManager(IGameState* pStartState);
	~GameStateManager();

	GameStateManager(const GameStateManager& other) = delete;
	GameStateManager(GameStateManager&& other) = delete;
	GameStateManager& operator=(const GameStateManager& other) = delete;
	GameStateManager& operator=(GameStateManager&& other) = delete;

	virtual void Notify(const dae::GameObject& gameObject, const Event& action) override;

private:
	IGameState* m_pCurrentState;
};

