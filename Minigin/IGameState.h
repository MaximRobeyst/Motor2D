#pragma once
class IGameState
{
public:
	virtual ~IGameState() = 0;

	virtual void OnEnter() = 0;
	virtual IGameState* Update() = 0;
	virtual void OnExit() = 0;
};

