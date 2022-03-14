#pragma once
#include "Component.h"

class dae::GameObject;
class HealthComponent final : public dae::Component
{
public:
	HealthComponent(dae::GameObject* pGameObject, int lives = 3);

	void Update() override;
	void Render() const override;

	void HitPoint();

private:
	int m_Lives;
};

