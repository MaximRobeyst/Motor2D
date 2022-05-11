#include "MiniginPCH.h"
#include "MrHotDogComponent.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "PlayerComponent.h"
#include "Event.h"

#include <Scene.h>
#include <ServiceLocator.h>

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
	if (m_pPlayerTransform == nullptr)
	{
		auto pPlayerobject = dae::SceneManager::GetInstance().GetScene(0)->FindGmeobjectWithTag("Player");
		if (pPlayerobject != nullptr)
		{
			m_pPlayerTransform = pPlayerobject->GetComponent<dae::TransformComponent>();
		}
	}

	m_pTransfomComponent = m_pGameObject->GetComponent<dae::TransformComponent>();
	m_pRigidbodyComponent = m_pGameObject->GetComponent<dae::RigidbodyComponent>();
	m_pAnimatorComponent = m_pGameObject->GetComponent<dae::AnimatorComponent>();

	m_pAnimatorComponent->SetAnimation("WalkLeft");

	auto rigidBodyComponent = m_pGameObject->GetComponent<dae::RigidbodyComponent>();

	std::function<void(dae::RigidbodyComponent*, dae::RigidbodyComponent*, b2Contact*)> newFunction = [](dae::RigidbodyComponent* pTriggeredbody, dae::RigidbodyComponent* otherBody, b2Contact*)
	{
		auto pOtherGO = otherBody->GetGameobject();

		if (pOtherGO->GetTag() == "Player")
		{
			auto playerComponent = pOtherGO->GetComponent<PlayerComponent>();
			playerComponent->PlayerDeath();
		}
		else if (pOtherGO->GetTag() == "Food")
		{
			auto enemyComp = pTriggeredbody->GetGameobject()->GetComponent<EnemyComponent>();
			enemyComp->EnemyDeath();
		}
	};

	rigidBodyComponent->SetOnEnterFunction(newFunction);

}

void EnemyComponent::Update()
{
	if (m_pPlayerTransform->GetPosition().x <= m_pTransfomComponent->GetPosition().x)
		m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ -m_Speed, 0.f });
	else if(m_pPlayerTransform->GetPosition().x >= m_pTransfomComponent->GetPosition().x)
		m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ m_Speed, 0.f });

	if (m_Dead)
	{
		if(m_pAnimatorComponent->IsAnimationDone())
			dae::SceneManager::GetInstance().GetScene(0)->RemoveGameObject(m_pGameObject);

		return;
	}
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
