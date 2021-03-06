#include "MiniginPCH.h"
#include "PlayerComponent.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "GameTime.h"
#include "LifeComponent.h"
#include <ServiceLocator.h>
#include <imgui.h>

#include <Collider.h>
#include "MrHotDogComponent.h"
#include <Scene.h>
#include <RaycastCallback.h>
#include <Renderer.h>

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

void PlayerComponent::Render() const
{
	b2Vec2 startPosition = b2Vec2{ m_pTranformComponent->GetPosition().x + 16.f, m_pTranformComponent->GetPosition().y + 16.f };
	b2Vec2 endPosition = startPosition + b2Vec2{ m_LastDirection.x * 32.f, m_LastDirection.y * 32.f };
	dae::Renderer::GetInstance().RenderLine(startPosition, endPosition, SDL_Color{ 255, 255, 0, 255 });
}

void PlayerComponent::RenderGUI()
{
}

void PlayerComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.Key("horizontalAxis");
	writer.String(m_pHorizontalAxis.c_str());
	writer.Key("VerticalAxis");
	writer.String(m_pVerticalAxis.c_str());
	writer.EndObject();
}

void PlayerComponent::Deserialize(dae::GameObject* pGameobject, rapidjson::Value& value)
{
	m_pGameObject = pGameobject;

	m_pHorizontalAxis = value["horizontalAxis"].GetString();
	m_pVerticalAxis = value["VerticalAxis"].GetString();
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

void PlayerComponent::SetVerticalAxis(const std::string& verticalAxis)
{
	m_pVerticalAxis = verticalAxis;
}

void PlayerComponent::SetHorizontalAxis(const std::string& horizontalAxis)
{
	m_pHorizontalAxis = horizontalAxis;
}

void PlayerComponent::SpawnPepper()
{
	if (m_Peppers == 0) return;
	m_CurrentState = PlayerState::State_Peper;
	

	m_pPepperGameobject = new dae::GameObject("Pepper");
	m_pPepperGameobject->AddComponent(new dae::TransformComponent(m_pPepperGameobject, m_pTranformComponent->GetPosition() +( m_LastDirection * 32.f ), glm::vec2{2.f}));
	m_pPepperGameobject->AddComponent(new dae::SpriteRendererComponent(m_pPepperGameobject, "BurgerTime_SpriteSheet.png", SDL_FRect{ 180, 16, 16, 16 }));
	auto pAnimation = new dae::AnimatorComponent(m_pPepperGameobject, "../Data/Animations/PepperAnimation.json");
	m_pPepperGameobject->AddComponent(pAnimation);
	pAnimation->SetAnimation("Idle");

	dae::RaycastCallback callback{ };

	b2Vec2 startPosition = b2Vec2{ m_pTranformComponent->GetPosition().x + 16.f, m_pTranformComponent->GetPosition().y + 16.f};
	b2Vec2 endPosition = startPosition + b2Vec2{ m_LastDirection.x * 32.f, m_LastDirection.y * 32.f };
	m_pGameObject->GetScene()->GetPhysicsWorld()->RayCast(&callback, startPosition, endPosition);
	if (dae::GameObject* lastHit = callback.GetLatestHit().pHitObject; lastHit != nullptr && lastHit->GetTag() == "Enemy")
	{
		lastHit->GetComponent<EnemyComponent>()->EnemyDeath();
	}

	m_pGameObject->GetScene()->AddGameObject(m_pPepperGameobject);

	--m_Peppers;


}

void PlayerComponent::UpdateDefault()
{
	auto keyboard = dae::InputManager::GetInstance().GetKeyboard();
	auto& transform = m_pTranformComponent->GetTransformConst();

	b2Vec2 vel{};

	assert(m_pHorizontalAxis != "" && m_pVerticalAxis != "");

	int horAxis = dae::InputManager::GetInstance().GetAxis(m_pHorizontalAxis)->GetAxisValue();
	int verAxis = dae::InputManager::GetInstance().GetAxis(m_pVerticalAxis)->GetAxisValue();


	if ((horAxis >= 0 && transform.scale.x > 0) || (horAxis <= -1 && transform.scale.x < 0)) m_pTranformComponent->GetTransform().scale.x *= -1;

	if (horAxis != 0)
	{
		m_pAnimatorComponent->SetAnimation("WalkLeft");
		vel.x = horAxis * 64.f /** GameTime::GetInstance()->GetElapsed()*/;

		m_LastDirection.x = static_cast<float>(horAxis);
		m_LastDirection.y = 0.f;
	}
	else if (verAxis != 0)
	{
		if (verAxis >= 1) m_pAnimatorComponent->SetAnimation("WalkDown");
		else if (verAxis <= -1) m_pAnimatorComponent->SetAnimation("WalkUp");

		vel .y = verAxis * 64.f /** GameTime::GetInstance()->GetElapsed()*/;

		m_LastDirection.x = 0.f;
		m_LastDirection.y = static_cast<float>(verAxis);
	}
	else m_pAnimatorComponent->SetAnimation("Idle");

	m_pRigidbody->GetBody()->SetLinearVelocity(vel);
}

void PlayerComponent::UpdatePeper()
{
	m_PepperTimer += GameTime::GetInstance()->GetElapsed();

	if (m_PepperTimer >= m_PepperTime)
	{
		m_PepperTimer = 0.0f;
		m_pGameObject->GetScene()->RemoveGameObject(m_pPepperGameobject);
		m_CurrentState = PlayerState::State_Default;
	}
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
