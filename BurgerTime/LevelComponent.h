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
	LevelComponent(dae::GameObject* m_pGameobject, int width, int height, std::vector<std::vector<char>> level, std::string nextLevel);

	void Start() override;
	void RenderGUI() override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer);
	void Deserialize(dae::GameObject* pGameobject, rapidjson::Value& /*value*/);

	void RemoveLevel();

	void AddPlates(PlateComponent* pPlateComponent);
	void AddIngredient(FoodComponent* pFoodcomponent);

	virtual void Notify(const dae::GameObject& gameObject, const Event& action) override;

	std::string GetNextLevel() const;


private:
	int m_Width{};
	int m_Height{};

	std::string m_NextLevel{};

	std::vector<std::vector<char>> m_Level{};

	std::vector<dae::GameObject*> m_pPlates{};
	std::vector<dae::GameObject*> m_pIngredients{};
};

