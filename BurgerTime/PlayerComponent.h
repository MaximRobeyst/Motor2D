#pragma once
#include "Component.h"
#include "RigidbodyComponent.h"
#include "AnimatorComponent.h"

#include <InputManager.h>

class LifeComponent;
class PlayerComponent : public dae::Component
{
	// I use the enum and switch aproach because making a state machine would be a bit overkill i think
	enum class PlayerState
	{
		State_Default,
		State_Peper,
		State_Dying,
	};

public:
	PlayerComponent() = default;
	PlayerComponent(dae::GameObject* pGameobject);
	~PlayerComponent() override = default;

	void Start() override;
	void Update() override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
	void Deserialize(dae::GameObject* /*pGameobject*/, rapidjson::Value& /*value*/) override;

	void PlayerDeath();
	bool IsDead() const;

	void SetVerticalAxis(dae::AxisManager* verticalAxis);
	void SetHorizontalAxis(dae::AxisManager* horizontalAxis);

private:
	void UpdateDefault();
	void UpdatePeper();
	void UpdateDying();

	PlayerState m_CurrentState;
	glm::vec3 m_StartPosition{};

	dae::RigidbodyComponent* m_pRigidbody;
	dae::TransformComponent* m_pTranformComponent;
	dae::AnimatorComponent* m_pAnimatorComponent;
	LifeComponent* m_pLifeComponent;

	dae::AxisManager* m_pHorizontalAxis{};
	dae::AxisManager* m_pVerticalAxis{};
};

