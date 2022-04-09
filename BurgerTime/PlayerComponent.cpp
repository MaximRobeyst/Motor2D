#include "MiniginPCH.h"
#include "PlayerComponent.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Transform.h"

PlayerComponent::PlayerComponent(dae::GameObject* pGameobject)
	:dae::Component(pGameobject)
{
	m_pRigidbody = pGameobject->GetComponent<dae::RigidbodyComponent>();
	m_pTranformComponent = pGameobject->GetComponent<dae::TransformComponent>();
	m_pAnimatorComponent = pGameobject->GetComponent<dae::AnimatorComponent>();
}

void PlayerComponent::Update()
{
	switch (m_CurrentState)
	{
	case PlayerState::State_Default:
		UpdateDefault();
		break;
	case PlayerState::State_Climbing:
		UpdateClimbing();
		break;
	case PlayerState::State_Dying:
		UpdateDying();
		break;
	}
}

void PlayerComponent::UpdateDefault()
{
	auto keyboard = dae::InputManager::GetInstance().GetKeyboard();

	// TODO: make axis support
	// TODO: give transform & rigidbody struct to make it easier to change values within them
	int keyPress = (int)(keyboard->IsPressed(SDLK_d) - keyboard->IsPressed(SDLK_a));
	m_pRigidbody->AddForce(glm::vec2{ (keyPress * 100.f), 0 });

	if ((keyPress >= 0 && m_pTranformComponent->GetScale().x > 0) || (keyPress <= -1 && m_pTranformComponent->GetScale().x < 0)) m_pTranformComponent->GetTransform().scale.x *= -1;

	if (keyPress != 0) m_pAnimatorComponent->SetAnimation("WalkLeft");
	else m_pAnimatorComponent->SetAnimation("Idle");
}

void PlayerComponent::UpdateClimbing()
{
}

void PlayerComponent::UpdateDying()
{
}
