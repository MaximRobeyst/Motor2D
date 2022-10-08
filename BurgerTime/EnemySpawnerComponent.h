#pragma once
#include <Component.h>
#include <Transform.h>
#include <Observer.h>

#include <vector>

class EnemySpawnerComponent : public dae::Component, Observer
{
public:
	EnemySpawnerComponent() = default;
	EnemySpawnerComponent(dae::GameObject* pGameobject);
	virtual ~EnemySpawnerComponent();
	EnemySpawnerComponent(const EnemySpawnerComponent& other) = delete;
	EnemySpawnerComponent(EnemySpawnerComponent&& other) = delete;
	EnemySpawnerComponent& operator=(const EnemySpawnerComponent& other) = delete;
	EnemySpawnerComponent& operator=(EnemySpawnerComponent&& other) = delete;

	void Notify(const dae::GameObject& gameObject, const Event& action) override;

	// Component functions
	void Start() override;
	void Update() override;
	void Render() const override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& /*writer*/);
	void Deserialize(dae::GameObject* /*pGameobject*/, rapidjson::Value& /*value*/);

private:
	dae::GameObject* SpawnEnemy(glm::vec3 position);

	int m_Count{};
	int m_MaxCount{ 4 };
	int m_CurrentIndex{};

	int m_AmountOfEnemies{ 1 };

	float m_BorderOffset{ 50.f };


	float m_TimebetweenSpawns{ 1.5f };
	float m_Timer{ 0.0f };

	std::vector<dae::GameObject*> m_SpawnedObjects;
	Observer* m_pScoreDisplay{};

};

