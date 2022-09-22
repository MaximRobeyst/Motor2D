#pragma once
#include "Component.h"
#include "RigidbodyComponent.h"
#include "AnimatorComponent.h"

#include <InputManager.h>

class LifeComponent;
class WeaponComponent;
class PlayerComponent : public dae::Component
{
	enum class PlayerState
	{
		State_Default,
		State_Attack,
		State_Dying,
	};

public:
	PlayerComponent() = default;
	PlayerComponent(dae::GameObject* pGameobject);
	~PlayerComponent() override = default;

	void Start() override;
	void Update() override;

	void RenderGUI() override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
	void Deserialize(dae::GameObject* /*pGameobject*/, rapidjson::Value& /*value*/) override;

	void PlayerDeath();
	bool IsDead() const;

	void SetVerticalAxis(const std::string& verticalAxis);
	void SetHorizontalAxis(const std::string& horizontalAxis);

	void Attack();

private:
	void UpdateDefault();
	void UpdateAttack();
	void UpdateDying();

	PlayerState m_CurrentState{};
	glm::vec3 m_StartPosition{};
	glm::vec3 m_LastDirection{};

	dae::RigidbodyComponent* m_pRigidbody{};
	dae::TransformComponent* m_pTranformComponent{};
	dae::AnimatorComponent* m_pAnimatorComponent{};
	LifeComponent* m_pLifeComponent{};

	dae::GameObject* m_pPepperGameobject{};

	WeaponComponent* m_pCurrentInteractable{};
	WeaponComponent* m_pCurrentWeapon{};
	
	float m_PepperTime{1.5f};
	float m_PepperTimer{};

	float m_Speed{100.0f};

	std::string m_pHorizontalAxis{};
	std::string m_pVerticalAxis{};
};

