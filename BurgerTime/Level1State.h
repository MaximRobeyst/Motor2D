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
	dae::GameObject* m_pPlayerObject;
	ScoreDisplayComponent* m_pScoreDisplay;
	GameManagerComponent* m_pManagerComponent;
};

class AttackCommand : public Command
{
public:
	AttackCommand() = default;
	AttackCommand(PlayerComponent* pMenuComp);
	void Execute() override;

	virtual void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>&) override;
	virtual void Deserialize(rapidjson::Value&, dae::Scene* pScene) override;
private:
	PlayerComponent* m_pPlayerComponent;
};

