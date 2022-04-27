#pragma once
#include "Component.h"
#include <memory>

class dae::GameObject;
class Subject;
class LifeComponent final : public dae::Component
{
public:
	LifeComponent(dae::GameObject* pGameObject, int lives = 3);

	void Update() override {};
	void Render() const override {};

	void Hit();

	int GetLives() const;

	std::unique_ptr<Subject>& GetSubject();

private:
	int m_Lives;
	std::unique_ptr<Subject> m_pSubject;
};

