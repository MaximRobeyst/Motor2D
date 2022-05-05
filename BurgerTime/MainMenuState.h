#pragma once
#include <IGameState.h>

class MainMenuState final : public IGameState
{
public:
	MainMenuState() = default;
	~MainMenuState() = default;

	virtual void OnEnter() override;
	virtual void OnExit() override;

};

