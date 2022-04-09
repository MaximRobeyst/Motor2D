#include "MiniginPCH.h"
#include "MovementComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Xbox360Controller.h"
#include "Time.h"

MovementComponent::MovementComponent(dae::GameObject* pGameObject, float speed)
	: Component{pGameObject}
	, m_Speed{speed}
{
	m_pTransformComponent = pGameObject->GetComponent<dae::TransformComponent>();
}

void MovementComponent::Update()
{
	//auto& input = dae::InputManager::GetInstance();
	//
	//glm::vec3 velocity{};
	//
	//if (input.GetController(0)->IsPressed(ControllerButton::DPadUp))
	//	velocity.y = -1;
	//if (input.GetController(0)->IsPressed(ControllerButton::DPadDown))
	//	velocity.y = 1;
	//if (input.GetController(0)->IsPressed(ControllerButton::DPadLeft))
	//	velocity.x = -1;
	//if (input.GetController(0)->IsPressed(ControllerButton::DPadRight))
	//	velocity.x = 1;
	//
	//velocity *= m_Speed * Time::GetInstance()->GetElapsed();
	//m_pTransformComponent->SetPosition(m_pTransformComponent->GetPosition() + velocity);
}

void MovementComponent::Render() const
{
}
