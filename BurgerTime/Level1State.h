#pragma once
#include <IGameState.h>

namespace dae
{
	class Scene;
	class GameObject;
}
class GameManagerComponent;
class ScoreDisplayComponent;

class SingleplayerState : public IGameState
{
public:
	SingleplayerState() = default;
	~SingleplayerState() = default;

	virtual void OnEnter() override;
	virtual void OnExit() override;

private:
	void MakeLevel(dae::Scene& pScene);
	void MakeLevel2(dae::Scene& pScene);
	void MakeLevel3(dae::Scene& pScene);

	dae::GameObject* m_pPlayerObject;
	ScoreDisplayComponent* m_pScoreDisplay;
	GameManagerComponent* m_pManagerComponent;
};

