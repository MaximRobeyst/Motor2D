#pragma once
#include <Component.h>
#include <Observer.h>

class LevelComponent;
class GameManagerComponent final : public dae::Component, public Observer
{
public:
	GameManagerComponent() = default;
	GameManagerComponent(dae::GameObject * pGameobject, dae::GameObject* pPlayer, int burgerAmount = 4);

	virtual void Notify(const dae::GameObject& gameObject, const Event& action) override;

	void SetLevelComponent(LevelComponent* pLevelComponent);
	void LoadNextLevel();

private:
	int m_CurrentBurgers;
	int m_AmountOfBurgers;

	int m_CurrentLevel{};
	std::vector < std::string> m_Levels{};

	dae::GameObject* m_pPlayerObject{};
	LevelComponent* m_pLevelComponent{};
};

