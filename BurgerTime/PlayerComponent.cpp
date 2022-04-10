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
	auto& transform = m_pTranformComponent->GetTransformConst();

	b2Vec2 vel{};

	// TODO: make axis support
	// TODO: give transform & rigidbody struct to make it easier to change values within them
	int horAxis = (int)(keyboard->IsPressed(SDLK_d) - keyboard->IsPressed(SDLK_a));
	int verAxis = (int)(keyboard->IsPressed(SDLK_s) - keyboard->IsPressed(SDLK_w));

	if ((horAxis >= 0 && transform.scale.x > 0) || (horAxis <= -1 && transform.scale.x < 0)) m_pTranformComponent->GetTransform().scale.x *= -1;

	if (horAxis != 0)
	{
		m_pAnimatorComponent->SetAnimation("WalkLeft");
		vel.x = horAxis * 64.f /** GameTime::GetInstance()->GetElapsed()*/;
	}
	else if (verAxis != 0)
	{
		if (verAxis >= 1) m_pAnimatorComponent->SetAnimation("WalkDown");
		else if (verAxis <= -1) m_pAnimatorComponent->SetAnimation("WalkUp");

		vel .y = verAxis * 100.f /** GameTime::GetInstance()->GetElapsed()*/;
	}
	else m_pAnimatorComponent->SetAnimation("Idle");

	m_pRigidbody->GetBody()->SetLinearVelocity(vel);
}

void PlayerComponent::UpdateClimbing()
{
}

void PlayerComponent::UpdateDying()
{
	m_pAnimatorComponent->SetAnimation("Death");
}
