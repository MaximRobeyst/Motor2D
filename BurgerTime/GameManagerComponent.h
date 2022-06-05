#pragma once
#include <Component.h>
#include <Observer.h>

class LevelComponent;
class GameManagerComponent final : public dae::Component, public Observer
{
public:
	GameManagerComponent() = default;
	GameManagerComponent(dae::GameObject * pGameobject, dae::GameObject* pPlayer, int burgerAmount = 4);

	void Start() override;
	void Update() override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
	void Deserialize(dae::GameObject* pGameobject, rapidjson::Value& /*value*/) override;

	virtual void Notify(const dae::GameObject& gameObject, const Event& action) override;

	void SetLevelComponent(LevelComponent* pLevelComponent);
	void LoadNextLevel();

private:
	int m_CurrentBurgers;
	int m_AmountOfBurgers;

	int m_CurrentLevel{};
	std::vector < std::string> m_Levels{};

	std::string m_NextLevel{};
	bool m_LoadNext{false};

	dae::GameObject* m_pPlayerObject{};
	LevelComponent* m_pLevelComponent{};
};

