#pragma once
#include <Component.h>
#include <Transform.h>

#include <map>

namespace dae
{
	class AnimatorComponent;
	class RigidbodyComponent;
	class TransformComponent;
	class ColliderComponent;
}

class Subject;
class GameStateManager;
class b2World;

class EnemyComponent : public dae::Component
{
public:
	EnemyComponent();
	EnemyComponent(dae::GameObject* pGameobject, dae::TransformComponent* pPlayerTransform, int score = 100);
	~EnemyComponent() override;

	void Start() override;
	void Update() override;
	void Render() const override;

	void RenderGUI() override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
	void Deserialize(dae::GameObject* /*pGameobject*/, rapidjson::Value& /*value*/) override;

	void Respawn(); 
	void EnemyDeath();

	std::unique_ptr<Subject>& GetSubject();
private:

	float m_Timer{ 0.0f };
	float m_TimeBetweenTransitionChecks{ 0.05f };

	int m_Score{100};
	bool m_Dead{};

	float m_MinDistance{ 0.25f };
	float m_Speed{ 1.f };

	dae::RigidbodyComponent* m_pRigidbodyComponent{};
	dae::TransformComponent* m_pTransformComponent{};
	dae::ColliderComponent* m_pColliderComponent{};

	dae::TransformComponent* m_pPlayerTransform{nullptr};
	std::unique_ptr<Subject> m_pSubject{};

	glm::vec3 m_PreviousPosition{};
	glm::vec2 m_CurrentTarget{};
};
