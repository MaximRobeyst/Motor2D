#pragma once
#include "Component.h"
#include "RigidbodyComponent.h"

class PlayerComponent : public dae::Component
{
	// I use the enum and switch aproach because making a state machine would be a bit overkill i think
	enum class PlayerState
	{
		State_Default,
		State_Climbing,
		State_Dying,
	};

public:
	PlayerComponent(dae::GameObject* pGameobject);
	~PlayerComponent() override = default;

	void Update() override;

private:
	void UpdateDefault();
	void UpdateClimbing();
	void UpdateDying();

	PlayerState m_CurrentState;
	dae::RigidbodyComponent* m_pRigidbody;
	dae::TransformComponent* m_pTranformComponent;

};

