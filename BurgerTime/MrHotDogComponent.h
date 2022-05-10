#pragma once
#include <Component.h>
#include <AnimatorComponent.h>
#include <Subject.h>
#include <Transform.h>

class EnemyComponent : public dae::Component
{
public:
	EnemyComponent(dae::GameObject* pGameobject, dae::TransformComponent* pPlayerTransform);

	void Update() override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;


	void EnemyDeath();
	
	int GetScore() const;

	std::unique_ptr<Subject>& GetSubject();
private:
	int m_Score{100};
	bool m_Dead{};

	float m_Speed{ 32.f };

	dae::AnimatorComponent* m_pAnimatorComponent;
	dae::RigidbodyComponent* m_pRigidbodyComponent;
	dae::TransformComponent* m_pTransfomComponent;

	dae::TransformComponent* m_pPlayerTransform;
	std::unique_ptr<Subject> m_pSubject;
};