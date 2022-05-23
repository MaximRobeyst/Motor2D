#pragma once
#include <IGameState.h>

class VersusState final : public IGameState
{
public:
	VersusState() = default;
	~VersusState() = default;

	virtual void OnEnter() override;
	virtual void OnExit() override;
};

