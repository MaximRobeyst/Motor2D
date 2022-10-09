#pragma once
#include "Component.h"
#include "Observer.h"
#include "TextObject.h"

class Subject;
class EnemySpawnerComponent;
class ScoreDisplayComponent : public dae::Component, public Observer
{
public:
	ScoreDisplayComponent();
	ScoreDisplayComponent(dae::GameObject* pGameObject, int number, const std::string& extraDisplayText = "");

	void Start() override;
	void Update() override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
	void Deserialize(dae::GameObject* pGameobject, rapidjson::Value& /*value*/) override;

	void Notify(const dae::GameObject& gameObject, const Event& action);
	void ChangeText(int number);

	static int GetScore();
	static void ResetScore();

	std::unique_ptr<Subject>& GetSubject();
private:
	void SpawnProjectile();
	void SpawnEnemy();

	dae::TextComponent* m_pTextComponent;
	std::string m_ExtraDisplayText;
	EnemySpawnerComponent* m_EnemySpawnerComponent;

	static int m_Score;

	float m_Timer;
	float m_TimerTillNextSpawn;

	std::unique_ptr<Subject> m_pSubject;
};

