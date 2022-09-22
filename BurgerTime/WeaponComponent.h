#pragma once
#include <Component.h>

class InteractComponent;

namespace dae
{
	class RigidbodyComponent;
	class TransformComponent;
}

class WeaponComponent : public dae::Component
{
public:
	WeaponComponent();

	void Start() override;
	void Update() override;

	void Interact(InteractComponent* m_pCaller);	// Pickup function
	void Attack(InteractComponent* m_pCaller);		// Shoot function


private:
	dae::RigidbodyComponent* m_pRigidbodyComponent{ nullptr };
	dae::TransformComponent* m_pTransformComponent{ nullptr };

	InteractComponent* m_pOwner{};
};

