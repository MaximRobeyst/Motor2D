#pragma once
#include "Component.h"
#include "RigidbodyComponent.h"
#include "AnimatorComponent.h"

#include <InputManager.h>

class LifeComponent;
class PlayerComponent : public dae::Component
{
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
	void Render() const override;

	void RenderGUI() override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
	void Deserialize(dae::GameObject* /*pGameobject*/, rapidjson::Value& /*value*/) override;

	void PlayerDeath();
	bool IsDead() const;

	void SetVerticalAxis(const std::string& verticalAxis);
	void SetHorizontalAxis(const std::string& horizontalAxis);

	void SpawnPepper();

private:
	void UpdateDefault();
	void UpdatePeper();
	void UpdateDying();

	PlayerState m_CurrentState{};
	glm::vec3 m_StartPosition{};
	glm::vec3 m_LastDirection{};

	dae::RigidbodyComponent* m_pRigidbody{};
	dae::TransformComponent* m_pTranformComponent{};
	dae::AnimatorComponent* m_pAnimatorComponent{};
	LifeComponent* m_pLifeComponent{};

	dae::GameObject* m_pPepperGameobject{};
	int m_Peppers{5};
	
	float m_PepperTime{1.5f};
	float m_PepperTimer{};

	std::string m_pHorizontalAxis{};
	std::string m_pVerticalAxis{};
};

