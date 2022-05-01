#pragma once
#include <IGameState.h>
#include <Scene.h>

class SingleplayerState : public IGameState
{
public:
	virtual void OnEnter() override;
	virtual IGameState* Update() override;
	virtual void OnExit() override;

private:
	void MakeLevel(dae::Scene& scene);
};

