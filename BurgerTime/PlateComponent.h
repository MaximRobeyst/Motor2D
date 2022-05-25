#pragma once
#include <Component.h>
#include <RigidbodyComponent.h>
#include <Subject.h>

class FoodComponent;
class PlateComponent : public dae::Component
{
public:
	PlateComponent() = default;
	PlateComponent(dae::GameObject* pGameobject);

	void Start() override;
	void Update() override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
	void Deserialize(dae::GameObject* /*pGameobject*/, rapidjson::Value& /*value*/) override;

	void AddIngredient(FoodComponent* pFoodComponent);

	std::unique_ptr<Subject>& GetSubject();
private:
	dae::RigidbodyComponent* m_pRigidbody{ nullptr };
	std::vector<FoodComponent*> m_pIngredients{};

	std::unique_ptr<Subject> m_pSubject;

	bool m_BurgerMade{ false };
};

