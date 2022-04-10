#pragma once
#include <Component.h>
#include <AnimatorComponent.h>

class EnemyComponent : public dae::Component
{
public:
	EnemyComponent(dae::GameObject* pGameobject);

	void Update() override;

	void EnemyDeath();
private:
	dae::AnimatorComponent* m_pAnimatorComponent;
};

