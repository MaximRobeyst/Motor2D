#pragma once
#include <IGameState.h>

class LeaderboardState : public IGameState
{
public:
	LeaderboardState(int score);
	~LeaderboardState() = default;

	virtual void OnEnter() override;
	virtual void OnExit() override;
private:
	int m_CurrentScore{ 0 };
};

