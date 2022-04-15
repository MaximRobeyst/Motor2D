#pragma once
#include <Component.h>
#include <RigidbodyComponent.h>

class PlateComponent : public dae::Component
{
public:
	PlateComponent(dae::GameObject* pGameobject);

	void Start() override;
	void Update() override;

	void AddIngredient(dae::GameObject* pGameobject);

private:
	dae::RigidbodyComponent* m_pRigidbody{ nullptr };
	std::vector<dae::GameObject*> m_pIngredients{};
};

