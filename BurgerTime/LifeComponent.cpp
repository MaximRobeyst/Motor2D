#include "LifeComponent.h"
#include "GameObject.h"
#include "Subject.h"
#include "Event.h"

LifeComponent::LifeComponent(dae::GameObject* pGameObject, int lives)
	: dae::Component(pGameObject)
	, m_Lives{lives}
	, m_pSubject{ std::make_unique<Subject>() }
{
}

void LifeComponent::Hit()
{
	if (m_Lives == 0)
		return;

	--m_Lives;

	m_pSubject->Notify(*m_pGameObject, Event::Player_Died);

	// Call observer event
}

int LifeComponent::GetLives() const
{
	return m_Lives;
}

std::unique_ptr<Subject>& LifeComponent::GetSubject()
{
	return m_pSubject;
}
