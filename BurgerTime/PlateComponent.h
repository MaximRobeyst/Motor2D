#pragma once
#include <Component.h>
#include <RigidbodyComponent.h>

class PlateComponent : public dae::Component
{
public:
	PlateComponent() = default;
	PlateComponent(dae::GameObject* pGameobject);

	void Start() override;
	void Update() override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
	void Deserialize(dae::GameObject* /*pGameobject*/, rapidjson::Value& /*value*/) override;

	void AddIngredient(dae::GameObject* pGameobject);

private:
	dae::RigidbodyComponent* m_pRigidbody{ nullptr };
	std::vector<dae::GameObject*> m_pIngredients{};
};

