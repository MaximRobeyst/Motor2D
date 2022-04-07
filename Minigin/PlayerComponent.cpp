#include "MiniginPCH.h"
#include "PlayerComponent.h"
#include "InputManager.h"
#include "GameObject.h"

PlayerComponent::PlayerComponent(dae::GameObject* pGameobject)
	:dae::Component(pGameobject)
{
	m_pRigidbody = pGameobject->GetComponent<dae::RigidbodyComponent>();
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
	m_pRigidbody->SetVelocity((int)(keyboard->IsPressed(SDLK_d) - keyboard->IsPressed(SDLK_a)) * 100.f * glm::vec2(1, 0));



}

void PlayerComponent::UpdateClimbing()
{
}

void PlayerComponent::UpdateDying()
{
}
