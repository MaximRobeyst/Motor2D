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

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;


	void Hit();

	int GetLives() const;

	std::unique_ptr<Subject>& GetSubject();

	void SetEnabled(bool newBool);

private:
	int m_Lives;
	bool m_Enabled = true;
	std::unique_ptr<Subject> m_pSubject;
	//PlayerComponent* m_pPlayerComp;
};

