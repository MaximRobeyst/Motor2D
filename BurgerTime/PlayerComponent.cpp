#include "MiniginPCH.h"
#include "PlayerComponent.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "GameTime.h"
#include "LifeComponent.h"
#include "WeaponComponent.h"
#include "InteractComponent.h"

#include <ServiceLocator.h>
#include <imgui.h>

#include <Collider.h>
#include "MrHotDogComponent.h"
#include <Scene.h>
#include <RaycastCallback.h>
#include <Renderer.h>
#include <CameraComponent.h>
#include <RapidjsonHelpers.h>

#include <GameStateManager.h>
#include "LeaderboardState.h"
#include "ScoreDisplayComponent.h"

dae::Creator<dae::Component, PlayerComponent> s_TranformComponentCreate{};

PlayerComponent::PlayerComponent(dae::GameObject* pGameobject)
	:dae::Component(pGameobject)
{
}

void PlayerComponent::Start()
{
	m_pRigidbody = m_pGameObject->GetComponent<dae::RigidbodyComponent>();
	m_pTranformComponent = m_pGameObject->GetComponent<dae::TransformComponent>();
	//m_pAnimatorComponent = m_pGameObject->GetComponent<dae::AnimatorComponent>();

	m_pInteractComponent = m_pGameObject->GetComponent<InteractComponent>();
	m_pLifeComponent = m_pGameObject->GetComponent<LifeComponent>();

	m_StartPosition = m_pTranformComponent->GetPosition();

	m_pCurrentWeapon = m_pGameObject->GetComponentInChildren<WeaponComponent>();

	//if (m_pVerticalAxis == nullptr)
	//{
	//	dae::InputManager::GetInstance().AddAxis("keyboard_vertical", new dae::KeyboardAxis(SDLK_s, SDLK_w, dae::InputManager::GetInstance().GetKeyboard()));
	//	SetVerticalAxis(dae::InputManager::GetInstance().GetAxis("keyboard_vertical"));
	//}
	//
	//if (m_pHorizontalAxis == nullptr)
	//{
	//	dae::InputManager::GetInstance().AddAxis("keyboard_horizontal", new dae::KeyboardAxis(SDLK_d, SDLK_a, dae::InputManager::GetInstance().GetKeyboard()));
	//	SetHorizontalAxis(dae::InputManager::GetInstance().GetAxis("keyboard_horizontal"));
	//}
}

void PlayerComponent::Update()
{
	switch (m_CurrentState)
	{
	case PlayerState::State_Attack:
		UpdateAttack();
	case PlayerState::State_Default:
		UpdateDefault();
		break;
	case PlayerState::State_Dying:
		UpdateDying();
		break;
	}
}

void PlayerComponent::RenderGUI()
{
}

void PlayerComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	rapidjson::SerializeValue(writer, "Name", std::string(typeid(*this).name()));
	rapidjson::SerializeValue(writer, "HorizontalAxis", m_pHorizontalAxis);
	rapidjson::SerializeValue(writer, "VerticalAxis", m_pVerticalAxis);
	writer.EndObject();
}

void PlayerComponent::Deserialize(dae::GameObject* pGameobject, rapidjson::Value& value)
{
	m_pGameObject = pGameobject;

	rapidjson::DeserializeValue(value, "HorizontalAxis", m_pHorizontalAxis);
	rapidjson::DeserializeValue(value, "VerticalAxis", m_pVerticalAxis);
}

void PlayerComponent::PlayerDeath()
{
	if (m_CurrentState == PlayerState::State_Dying) return;

	ServiceLocator::GetAudio()->PlaySound("../Data/Audio/death_1.wav");

	GameStateManager::GetInstance().SwitchGameState(new LeaderboardState(ScoreDisplayComponent::GetScore()));
}

bool PlayerComponent::IsDead() const
{
	return m_CurrentState == PlayerState::State_Dying;
}

void PlayerComponent::SetVerticalAxis(const std::string& verticalAxis)
{
	m_pVerticalAxis = verticalAxis;
}

void PlayerComponent::SetHorizontalAxis(const std::string& horizontalAxis)
{
	m_pHorizontalAxis = horizontalAxis;
}

void PlayerComponent::Attack()
{
	m_pCurrentWeapon->Attack();
}

void PlayerComponent::UpdateDefault()
{
	auto position = m_pTranformComponent->GetPosition();
	auto keyboard = dae::InputManager::GetInstance().GetKeyboard();

	auto mousePosition = dae::InputManager::GetInstance().GetMousePosition();
	float angle = atan2f(mousePosition.y - position.y, mousePosition.x - position.x);

	m_pCurrentWeapon->GetGameObject()->GetTransform()->SetRotation(angle);
	m_pCurrentWeapon->GetGameObject()->GetTransform()->SetPosition(glm::vec3{ cosf(angle) * m_WeaponDistance, sinf(angle) * m_WeaponDistance, 0});


	//m_pTranformComponent->SetRotation(atan2f(mousePosition.y - transform.position.y, mousePosition.x - transform.position.x));

	assert(m_pHorizontalAxis != "" && m_pVerticalAxis != "");
	b2Vec2 vel{};

	int horAxis = dae::InputManager::GetInstance().GetAxis(m_pHorizontalAxis)->GetAxisValue();
	int verAxis = dae::InputManager::GetInstance().GetAxis(m_pVerticalAxis)->GetAxisValue();

	vel.x = horAxis * m_Speed;
	vel.y = verAxis * m_Speed;

	m_LastDirection.x = static_cast<float>(horAxis);
	m_LastDirection.y = static_cast<float>(verAxis);

	m_pRigidbody->GetBody()->SetLinearVelocity(vel);

	if (m_CurrentState == PlayerState::State_Attack) return;
}

void PlayerComponent::UpdateAttack()
{
}

void PlayerComponent::UpdateDying()
{
	//m_pAnimatorComponent->SetAnimation("Death");
	//if (m_pAnimatorComponent->IsAnimationDone())
	//{
	//	m_pLifeComponent->SetEnabled(true);
	//	m_pTranformComponent->SetPosition(m_StartPosition);
	//	m_CurrentState = PlayerState::State_Default;
	//}
}
