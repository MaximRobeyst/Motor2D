#include "MiniginPCH.h"
#include "PlayerComponent.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "GameTime.h"

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

void PlayerComponent::PlayerDeath()
{
	m_CurrentState = PlayerState::State_Dying;
}

void PlayerComponent::UpdateDefault()
{
	auto keyboard = dae::InputManager::GetInstance().GetKeyboard();
	auto& transform = m_pTranformComponent->GetTransform();

	// TODO: make axis support
	// TODO: give transform & rigidbody struct to make it easier to change values within them
	int horAxis = (int)(keyboard->IsPressed(SDLK_d) - keyboard->IsPressed(SDLK_a));
	int verAxis = (int)(keyboard->IsPressed(SDLK_s) - keyboard->IsPressed(SDLK_w));

	if ((horAxis >= 0 && transform.scale.x > 0) || (horAxis <= -1 && transform.scale.x < 0)) transform.scale.x *= -1;

	if (horAxis != 0)
	{
		m_pAnimatorComponent->SetAnimation("WalkLeft");
		transform.position.x += horAxis * 100.f * GameTime::GetInstance()->GetElapsed();
	}
	else if (verAxis != 0)
	{

		transform.position.y += verAxis * 100.f * GameTime::GetInstance()->GetElapsed();
	}
	else m_pAnimatorComponent->SetAnimation("Idle");
}

void PlayerComponent::UpdateClimbing()
{
}

void PlayerComponent::UpdateDying()
{
	m_pAnimatorComponent->SetAnimation("Death");
}
