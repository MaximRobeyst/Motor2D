#pragma once
#include <Component.h>

namespace dae
{
	class RigidbodyComponent;
}

class ProjectileComponent : public dae::Component
{
public:
	ProjectileComponent() = default;

	void Start() override;
	void Update() override;

	void RenderGUI() override;

private:
	dae::RigidbodyComponent* m_pRigidbodyComponent;

	float m_TimeAlive = 5.0f;
	float m_Speed = 500.0f;
	float m_CurrentTimer = 0.0f;

};

