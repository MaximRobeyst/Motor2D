#pragma once
#include <Component.h>
#include <Transform.h>

namespace dae
{
	class AnimatorComponent;
	class RigidbodyComponent;
	class TransformComponent;
	class ColliderComponent;

}

class Subject;

class EnemyComponent : public dae::Component
{
public:
	EnemyComponent();
	EnemyComponent(dae::GameObject* pGameobject, dae::TransformComponent* pPlayerTransform);

	void Start() override;
	void Update() override;
	void Render() const override;

	void RenderGUI() override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
	void Deserialize(dae::GameObject* /*pGameobject*/, rapidjson::Value& /*value*/) override;

	void EnemyDeath();
	
	int GetScore() const;

	std::unique_ptr<Subject>& GetSubject();
private:
	void UpdateWalk();
	void UpdateClimb();
	void UpdatePlayerDeath();
	void ChooseNextTarget();

	bool SpaceAbove();
	bool SpaceBelow();
	bool SpaceLeft();
	bool SpaceRight();

	int m_Score{100};
	bool m_Dead{};

	float m_Speed{ 32.f };
	float m_MinDistance{ 0.25f };

	dae::AnimatorComponent* m_pAnimatorComponent;
	dae::RigidbodyComponent* m_pRigidbodyComponent;
	dae::TransformComponent* m_pTransfomComponent;
	dae::ColliderComponent* m_pColliderComponent;

	dae::TransformComponent* m_pPlayerTransform{nullptr};
	std::unique_ptr<Subject> m_pSubject;

	glm::vec2 m_CurrentDirection{};
	glm::vec2 m_CurrentTarget{};

	// State machine
	enum class EnemyState
	{
		Walk,
		Climb,
		Playerdead,
		ChooseTarget
	};

	EnemyState m_CurrentState{ EnemyState::Walk };
};

