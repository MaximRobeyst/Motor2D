#include "MiniginPCH.h"
#include "HealthComponent.h"
#include "GameObject.h"

HealthComponent::HealthComponent(dae::GameObject* pGameObject, int lives)
	: dae::Component(pGameObject)
	, m_Lives{lives}
{
}

void HealthComponent::Update()
{
}

void HealthComponent::Render() const
{
}

void HealthComponent::HitPoint()
{
	--m_Lives;
	// Call observer event
}
