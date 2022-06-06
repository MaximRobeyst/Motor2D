#pragma once
#include <IGameState.h>
#include <Command.h>
#include <prettywriter.h>

#include <rapidjson.h>
#include <document.h>

namespace dae
{
	class Scene;
	class GameObject;
}
class GameManagerComponent;
class ScoreDisplayComponent;
class PlayerComponent;

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

class PepperCommand : public Command
{
public:
	PepperCommand() = default;
	PepperCommand(PlayerComponent* pMenuComp);
	void Execute() override;

	virtual void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>&) override;
	virtual void Deserialize(rapidjson::Value&, dae::Scene* pScene) override;
private:
	PlayerComponent* m_pPlayerComponent;
};

