#include "ProjectileComponent.h"

#include <GameObject.h>

#include <RigidbodyComponent.h>
#include <Transform.h>
#include <GameTime.h>

#include <imgui_helpers.h>

void ProjectileComponent::Start()
{
	m_pRigidbodyComponent = m_pGameObject->GetComponent<dae::RigidbodyComponent>();

	auto forward = m_pGameObject->GetTransform()->GetForward();
	m_pRigidbodyComponent->GetBody()->SetLinearVelocity(b2Vec2{forward.x, forward.y});
}

void ProjectileComponent::Update()
{
	m_CurrentTimer += GameTime::GetInstance()->GetElapsed();

	if (m_CurrentTimer >= m_TimeAlive)
	{
		dae::GameObject::Destroy(m_pGameObject);
	}
}

void ProjectileComponent::RenderGUI()
{
	ImGui::InputValue("TimeAlive", m_TimeAlive);
	ImGui::InputValue("Speed", m_Speed);
}
