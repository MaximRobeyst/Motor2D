#include "ProjectileComponent.h"
#include "PlayerComponent.h"
#include "MrHotDogComponent.h"

#include <GameObject.h>

#include <RigidbodyComponent.h>
#include <Transform.h>
#include <GameTime.h>

#include <imgui_helpers.h>

ProjectileComponent::ProjectileComponent(float speed, bool bounce)
	: m_Speed{speed}
	, m_Bounce{bounce}
{
}

void ProjectileComponent::Start()
{
	m_pRigidbodyComponent = m_pGameObject->GetComponent<dae::RigidbodyComponent>();
	std::function<void(dae::RigidbodyComponent*, dae::RigidbodyComponent*, b2Contact*)> newFunction = [this](dae::RigidbodyComponent* pTriggeredbody, dae::RigidbodyComponent* otherBody, b2Contact* contact)
	{
		auto pOtherGO = otherBody->GetGameObject();

		if (pOtherGO->GetTag() == "Player")
		{
			auto playerComponent = pOtherGO->GetComponent<PlayerComponent>();
			playerComponent->PlayerDeath();
		}
		else if (pOtherGO->GetTag() == "Enemy")
		{
			auto enemyComp = pTriggeredbody->GetGameObject()->GetComponent<EnemyComponent>();
			enemyComp->EnemyDeath();
		}

		if (!m_Bounce) dae::GameObject::Destroy(m_pGameObject);
		else
		{
			auto normal = contact->GetManifold()->localNormal;
			m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{ 
				m_pGameObject->GetTransform()->GetForward().x * normal.x * m_Speed, 
				m_pGameObject->GetTransform()->GetForward().y * normal.y * m_Speed });
		}
	};
	m_pRigidbodyComponent->SetOnEnterFunction(newFunction);

	auto forward = m_pGameObject->GetTransform()->GetForward();
	m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{forward.x * m_Speed, forward.y * m_Speed});
}

void ProjectileComponent::Update()
{
	m_CurrentTimer += GameTime::GetInstance()->GetElapsed();

	if (m_CurrentTimer >= m_TimeAlive && !m_Bounce)
	{
		dae::GameObject::Destroy(m_pGameObject);
	}
}

void ProjectileComponent::RenderGUI()
{
	ImGui::InputValue("TimeAlive", m_TimeAlive);
	ImGui::InputValue("Speed", m_Speed);
}
