#include "InteractComponent.h"
#include "WeaponComponent.h"

InteractComponent::InteractComponent()
{
}

void InteractComponent::Start()
{
}

void InteractComponent::SetCurrentInteractible(WeaponComponent* pCurrentInteractible)
{
	m_pCurrentInteractible = pCurrentInteractible;
}

void InteractComponent::InteractWithCurrentInteractable()
{
	if (m_pCurrentInteractible == nullptr) return;
	m_pCurrentInteractible->Interact(this);
}

WeaponComponent* InteractComponent::GetInteractable()
{
	return m_pCurrentInteractible;
}
