#pragma once
#include "IGameState.h"

class MainMenuState : public IGameState
{
public:
	MainMenuState() = default;

	virtual void OnEnter() override;
	virtual IGameState* Update() override;
	virtual void OnExit() override;

};

