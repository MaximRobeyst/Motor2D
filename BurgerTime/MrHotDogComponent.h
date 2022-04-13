#pragma once
#include <Component.h>
#include <AnimatorComponent.h>
#include <Subject.h>

class EnemyComponent : public dae::Component
{
public:
	EnemyComponent(dae::GameObject* pGameobject);

	void Update() override;

	void EnemyDeath();
	
	int GetScore() const;

	std::unique_ptr<Subject>& GetSubject();
private:
	int m_Score{100};
	bool m_Dead{};

	dae::AnimatorComponent* m_pAnimatorComponent;
	std::unique_ptr<Subject> m_pSubject;
};

