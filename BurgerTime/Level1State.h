#pragma once
#include <IGameState.h>

namespace dae
{
	class Scene;
}

class Level1State : public IGameState
{
public:
	Level1State() = default;
	~Level1State() = default;

	virtual void OnEnter() override;
	virtual void OnExit() override;

private:
	void MakeLevel(dae::Scene& pScene);
};

