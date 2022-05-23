#pragma once
#include <IGameState.h>

class MultiplayerState final : public IGameState
{
public:
	MultiplayerState() = default;
	~MultiplayerState() = default;

	virtual void OnEnter() override;
	virtual void OnExit() override;
};

