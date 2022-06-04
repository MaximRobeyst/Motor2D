#pragma once
#include <Component.h>
#include <string>
#include <vector>
#include <Observer.h>

namespace dae
{
	class GameObject;
}
class PlateComponent;
class FoodComponent;

class LevelComponent : public dae::Component, Observer
{
public:
	LevelComponent() = default;
	LevelComponent(dae::GameObject* m_pGameobject, int width, int height, std::vector<std::vector<char>> level);

	void RenderGUI() override;

	void RemoveLevel();

	void AddPlates(PlateComponent* pPlateComponent);
	void AddIngredient(FoodComponent* pFoodcomponent);

	virtual void Notify(const dae::GameObject& gameObject, const Event& action) override;


private:
	int m_Width{};
	int m_Height{};

	std::vector<std::vector<char>> m_Level{};

	std::vector<dae::GameObject*> m_pPlates{};
	std::vector<dae::GameObject*> m_pIngredients{};
};

