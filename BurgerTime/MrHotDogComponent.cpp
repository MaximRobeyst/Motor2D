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
#include <string>

#include <Subject.h>
#include <GameTime.h>

#include <Observer.h>

const dae::Creator<dae::Component, EnemyComponent> g_EnemyComponentCreator{};

EnemyComponent::EnemyComponent()
	: m_pSubject{std::make_unique<Subject>()}
{
}

EnemyComponent::EnemyComponent(dae::GameObject* pGameobject, dae::TransformComponent* pPlayerTransform, int score)
	: dae::Component(pGameobject)
	, m_pSubject{ std::make_unique<Subject>()}
	, m_pPlayerTransform{pPlayerTransform}
	, m_Score{score}
{
}

EnemyComponent::~EnemyComponent()
{
}

void EnemyComponent::Start()
{
	m_pTransformComponent = m_pGameObject->GetComponent<dae::TransformComponent>();
	m_pRigidbodyComponent = m_pGameObject->GetComponent<dae::RigidbodyComponent>();
	m_pColliderComponent = m_pGameObject->GetComponent < dae::ColliderComponent>();

	m_CurrentTarget = glm::vec2{ m_pTransformComponent->GetPosition() };

	std::function<void(dae::RigidbodyComponent*, dae::RigidbodyComponent*, b2Contact*)> newFunction = [this](dae::RigidbodyComponent* /*pTriggeredbody*/, dae::RigidbodyComponent* otherBody, b2Contact*)
	{
		auto pOtherGO = otherBody->GetGameObject();

		if (pOtherGO->GetTag() == "Player")
		{
			auto playerComponent = pOtherGO->GetComponent<PlayerComponent>();
			playerComponent->PlayerDeath();
		}
		else if (pOtherGO->GetTag() == "Projectile")
		{
			EnemyDeath();
			dae::GameObject::Destroy(pOtherGO);
		}
	};

	m_pRigidbodyComponent->SetOnEnterFunction(newFunction);

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
	auto direction = m_pPlayerTransform->GetTransform().position - m_pTransformComponent->GetPosition();
	glm::normalize(direction);
	direction *= m_Speed;

	m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{direction.x, direction.y});
}

void EnemyComponent::Render() const
{
}

void EnemyComponent::RenderGUI()
{
}

void EnemyComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.Key("Observers");
	writer.StartArray();
	for (int i = 0; i < m_pSubject->GetNrOfObservers(); ++i)
	{
		writer.Int(m_pSubject->GetObserverFromId(i)->GetId());
	}
	writer.EndArray();
	writer.EndObject();
}

void EnemyComponent::Deserialize(dae::GameObject* pGameObject, rapidjson::Value& value)
{
	m_pGameObject = pGameObject;

	for (auto iter = value["Observers"].Begin(); iter != value["Observers"].End(); ++iter)
	{
		auto pObserver = pGameObject->GetScene()->GetGameobjectFromId(iter->GetInt());

		m_pSubject->AddObserver(pObserver->GetComponent<Observer>());

	}
}

void EnemyComponent::Respawn()
{
	m_Dead = false;
}

void EnemyComponent::EnemyDeath()
{
	ServiceLocator::GetAudio()->PlaySound("../Data/Audio/death_2.wav");
	m_pSubject->Notify(*m_pGameObject, Event::Enemy_Died);
	m_pGameObject->SetEnabled(false);
	m_Dead = true;
}

std::unique_ptr<Subject>& EnemyComponent::GetSubject()
{
	return m_pSubject;
}