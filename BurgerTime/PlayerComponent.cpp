#include "MiniginPCH.h"
#include "PlayerComponent.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "GameTime.h"
#include "LifeComponent.h"
#include <ServiceLocator.h>

dae::Creator<dae::Component, PlayerComponent> s_TranformComponentCreate{};

PlayerComponent::PlayerComponent(dae::GameObject* pGameobject)
	:dae::Component(pGameobject)
{
}

void PlayerComponent::Start()
{
	m_pRigidbody = m_pGameObject->GetComponent<dae::RigidbodyComponent>();
	m_pTranformComponent = m_pGameObject->GetComponent<dae::TransformComponent>();
	m_pAnimatorComponent = m_pGameObject->GetComponent<dae::AnimatorComponent>();
	m_pLifeComponent = m_pGameObject->GetComponent<LifeComponent>();

	m_StartPosition = m_pTranformComponent->GetPosition();
}

void PlayerComponent::Update()
{
	switch (m_CurrentState)
	{
	case PlayerState::State_Default:
		UpdateDefault();
		break;
	case PlayerState::State_Peper:
		UpdatePeper();
		break;
	case PlayerState::State_Dying:
		UpdateDying();
		break;
	}
}

void PlayerComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.EndObject();
}

void PlayerComponent::Deserialize(dae::GameObject* pGameobject, rapidjson::Value& value)
{
	m_pGameObject = pGameobject;
}

void PlayerComponent::PlayerDeath()
{
	if (m_CurrentState == PlayerState::State_Dying) return;

	m_CurrentState = PlayerState::State_Dying;
	m_pLifeComponent->Hit();
	m_pLifeComponent->SetEnabled(false);
	ServiceLocator::GetAudio()->PlaySound("../Data/Audio/death_1.wav");
}

bool PlayerComponent::IsDead() const
{
	return m_CurrentState == PlayerState::State_Dying;
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

		vel .y = verAxis * 64.f /** GameTime::GetInstance()->GetElapsed()*/;
	}
	else m_pAnimatorComponent->SetAnimation("Idle");

	m_pRigidbody->GetBody()->SetLinearVelocity(vel);
}

void PlayerComponent::UpdatePeper()
{
}

void PlayerComponent::UpdateDying()
{
	m_pAnimatorComponent->SetAnimation("Death");
	if (m_pAnimatorComponent->IsAnimationDone())
	{
		m_pLifeComponent->SetEnabled(true);
		m_pTranformComponent->SetPosition(m_StartPosition);
		m_CurrentState = PlayerState::State_Default;
	}
}
