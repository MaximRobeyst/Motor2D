#pragma once
#include "Component.h"

class WeaponComponent;

class InteractComponent : public dae::Component
{
public:
	InteractComponent();

	void Start() override;


	// Interaction with props
	void SetCurrentInteractible(WeaponComponent* pCurrentInteractible);
	void InteractWithCurrentInteractable();

private:
	WeaponComponent* m_pCurrentInteractible;
};

