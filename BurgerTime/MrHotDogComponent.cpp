#include "MiniginPCH.h"
#include "MrHotDogComponent.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "PlayerComponent.h"
#include "Event.h"

#include <Scene.h>
#include <ServiceLocator.h>
#include <RaycastCallback.h>
#include <Renderer.h>
#include <AnimatorComponent.h>
#include <Collider.h>

#include <Subject.h>

dae::Creator<dae::Component, EnemyComponent> s_TranformComponentCreate{};

EnemyComponent::EnemyComponent()
	: dae::Component()
	, m_pSubject{ std::make_unique<Subject>() }
{
}

EnemyComponent::EnemyComponent(dae::GameObject* pGameobject, dae::TransformComponent* pPlayerTransform)
	: dae::Component(pGameobject)
	, m_pSubject{ std::make_unique<Subject>()}
	, m_pPlayerTransform{pPlayerTransform}
{
}

void EnemyComponent::Start()
{
	m_pTransfomComponent = m_pGameObject->GetComponent<dae::TransformComponent>();
	m_pRigidbodyComponent = m_pGameObject->GetComponent<dae::RigidbodyComponent>();
	m_pAnimatorComponent = m_pGameObject->GetComponent<dae::AnimatorComponent>();
	m_pColliderComponent = m_pGameObject->GetComponent < dae::ColliderComponent>();

	m_pAnimatorComponent->SetAnimation("WalkLeft");

	auto rigidBodyComponent = m_pGameObject->GetComponent<dae::RigidbodyComponent>();

	std::function<void(dae::RigidbodyComponent*, dae::RigidbodyComponent*, b2Contact*)> newFunction = [](dae::RigidbodyComponent* pTriggeredbody, dae::RigidbodyComponent* otherBody, b2Contact*)
	{
		auto pOtherGO = otherBody->GetGameObject();

		if (pOtherGO->GetTag() == "Player")
		{
			auto playerComponent = pOtherGO->GetComponent<PlayerComponent>();
			playerComponent->PlayerDeath();
		}
		else if (pOtherGO->GetTag() == "Food")
		{
			auto enemyComp = pTriggeredbody->GetGameObject()->GetComponent<EnemyComponent>();
			enemyComp->EnemyDeath();
		}
	};

	rigidBodyComponent->SetOnEnterFunction(newFunction);

	if (m_pPlayerTransform == nullptr)
	{
		auto pPlayerobject = m_pGameObject->GetScene()->FindGameobjectWithTag("Player");
		if (pPlayerobject != nullptr)
		{
			m_pPlayerTransform = pPlayerobject->GetComponent<dae::TransformComponent>();
		}
	}

}

void EnemyComponent::Update()
{
	switch (m_CurrentState)
	{
	case EnemyState::Walk:
		UpdateWalk();
		break;
	case EnemyState::Climb:
		UpdateClimb();
	case EnemyState::Playerdead:
		UpdatePlayerDeath();
	}
	return;


	if (m_Dead)
	{
		if (m_pAnimatorComponent->IsAnimationDone())
			dae::SceneManager::GetInstance().GetScene(0)->RemoveGameObject(m_pGameObject);

		return;
	}

	//if(m_pPlayerTransform->GetPosition().y > 0)

	//if (m_pPlayerTransform == nullptr) return;
	//if (m_pPlayerTransform->GetPosition().x <= m_pTransfomComponent->GetPosition().x)
	//	m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ -m_Speed, 0.f });
	//else if(m_pPlayerTransform->GetPosition().x >= m_pTransfomComponent->GetPosition().x)
	//	m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ m_Speed, 0.f });

}

void EnemyComponent::Render() const
{
	auto pos = m_pTransfomComponent->GetPosition();
	b2Vec2 startPosition{ pos.x, pos.y + 1.0f };
	b2Vec2 endPosition{ startPosition + b2Vec2{0,-2.f} };
	dae::Renderer::GetInstance().RenderLine(startPosition, endPosition, glm::vec4{ 255.f, 0.f, 0.f, 255.f }, 2.0f);

	startPosition = b2Vec2 {pos.x + 1.0f, pos.y };
	endPosition = b2Vec2{ startPosition + b2Vec2{-2.f,0.f} };
	dae::Renderer::GetInstance().RenderLine(startPosition, endPosition, glm::vec4{ 255.f, 0.f, 0.f, 255.f }, 2.0f);
}

void EnemyComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.EndObject();
}

void EnemyComponent::Deserialize(dae::GameObject* pGameObject, rapidjson::Value&)
{
	m_pGameObject = pGameObject;
}

void EnemyComponent::EnemyDeath()
{
	ServiceLocator::GetAudio()->PlaySound("../Data/Audio/death_2.wav");
	m_pAnimatorComponent->SetAnimation("Death");
	m_pSubject->Notify(*m_pGameObject, Event::Enemy_Died);
	m_Dead = true;
}

int EnemyComponent::GetScore() const
{
	return m_Score;
}

std::unique_ptr<Subject>& EnemyComponent::GetSubject()
{
	return m_pSubject;
}

void EnemyComponent::UpdateWalk()
{
	// Raycast up to check if we can climb a ladder
	bool spaceAbove = SpaceAbove();
	//bool spaceBelow = SpaceBelow();

	//std::cout << "My position ["
	//	<< m_pTransfomComponent->GetPosition().x << " , " << m_pTransfomComponent->GetPosition().y << " ]" << std::endl << 
	//	"The players Position[" << m_pPlayerTransform->GetPosition().x << " , " << m_pPlayerTransform->GetPosition().y << " ] " << std::endl << 
	//	"Space above head: ?" << spaceAbove << " " << std::endl;

	if (spaceAbove && fabsf((m_pTransfomComponent->GetPosition().y - m_pPlayerTransform->GetPosition().y)) > 1.0f)
	{
		//m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ 0.f, -m_Speed });
		m_CurrentState = EnemyState::Climb;
		return;
	}
	m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ -m_Speed, 0.f });
}

void EnemyComponent::UpdateClimb()
{
	// Raycast up to check if we can climb a ladder
	if (!SpaceAbove())
	{
		if (m_pPlayerTransform->GetPosition().x < m_pTransfomComponent->GetPosition().x)
			m_CurrentDirection.x = -1.f;
		else
			m_CurrentDirection.x = 1.f;
	}

	if(SpaceLeft() && m_CurrentDirection.x)
		m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ 0.f, -m_Speed });
}

void EnemyComponent::UpdatePlayerDeath()
{
}

bool EnemyComponent::SpaceAbove()
{
	dae::RaycastCallback raycastCallback;
	const auto pos = m_pTransfomComponent->GetPosition();
	b2Vec2 startPosition{ pos.x, pos.y + 1.0f };
	b2Vec2 endPosition{ startPosition + b2Vec2{0,-3.f} };

	int sections = 3;
	float sizeDevidedBySections = m_pColliderComponent->GetSize().x / (sections- 1.f);

	for (int i = 0; i < sections; ++i)
	{
		m_pGameObject->GetScene()->GetPhysicsWorld()->RayCast(&raycastCallback, startPosition + b2Vec2{ sizeDevidedBySections * i, 0.f }, endPosition + b2Vec2{ sizeDevidedBySections * i, 0.f });
		auto hit = raycastCallback.GetLatestHit();

		std::cout << "hit " << std::to_string(i) << ": " << (hit.pHitObject ? hit.pHitObject->GetName() : "NULL") << std::endl;
		if (hit.pHitObject != nullptr)
			return false;
	}
	return true;
}

bool EnemyComponent::SpaceBelow()
{
	dae::RaycastCallback raycastCallback;
	const auto pos = m_pTransfomComponent->GetPosition();
	b2Vec2 startPosition{ pos.x, pos.y + 1.0f };
	b2Vec2 endPosition{ startPosition + b2Vec2{0,-2.f} };

	int sections = 3;
	float sizeDevidedBySections = m_pColliderComponent->GetSize().x / sections;

	for (int i = 0; i < sections; ++i)
	{
		m_pGameObject->GetScene()->GetPhysicsWorld()->RayCast(&raycastCallback, startPosition +b2Vec2{sizeDevidedBySections * i, 0.f}, endPosition + b2Vec2{sizeDevidedBySections * i, 0.f});
		auto hit = raycastCallback.GetLatestHit();

		std::cout << "hit " << std::to_string(i) << ": " << (hit.pHitObject ? hit.pHitObject->GetName() : "NULL") << std::endl;
		if (hit.pHitObject != nullptr)
			return false;
	}
	return true;
}

bool EnemyComponent::SpaceLeft()
{
	dae::RaycastCallback raycastCallback;
	const auto pos = m_pTransfomComponent->GetPosition();
	b2Vec2 startPosition{ pos.x, pos.y - 1.0f };
	b2Vec2 endPosition{ startPosition + b2Vec2{0,2.f} };

	int sections = 3;
	float sizeDevidedBySections = m_pColliderComponent->GetSize().x / sections;

	for (int i = 0; i < sections; ++i)
	{
		m_pGameObject->GetScene()->GetPhysicsWorld()->RayCast(&raycastCallback, startPosition + b2Vec2{ sizeDevidedBySections * i, 0.f }, endPosition + b2Vec2{ sizeDevidedBySections * i, 0.f });
		auto hit = raycastCallback.GetLatestHit();

		std::cout << "hit " << std::to_string(i) << ": " << (hit.pHitObject ? hit.pHitObject->GetName() : "NULL") << std::endl;
		if (hit.pHitObject != nullptr)
			return false;
	}
	return true;
}

bool EnemyComponent::SpaceRight()
{
	dae::RaycastCallback raycastCallback;
	const auto pos = m_pTransfomComponent->GetPosition();
	b2Vec2 startPosition{ pos.x + 1.0f, pos.y };
	b2Vec2 endPosition{ startPosition + b2Vec2{-2.f, 0.f} };

	int sections = 3;
	float sizeDevidedBySections = m_pColliderComponent->GetSize().x / sections;

	for (int i = 0; i < sections; ++i)
	{
		m_pGameObject->GetScene()->GetPhysicsWorld()->RayCast(&raycastCallback, startPosition + b2Vec2{ 0.f, sizeDevidedBySections * i}, endPosition + b2Vec2{ 0.f,  sizeDevidedBySections * i });
		auto hit = raycastCallback.GetLatestHit();

		std::cout << "hit " << std::to_string(i) << ": " << (hit.pHitObject ? hit.pHitObject->GetName() : "NULL") << std::endl;
		if (hit.pHitObject != nullptr)
			return false;
	}
	return true;
}
