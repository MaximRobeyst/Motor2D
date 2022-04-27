#pragma once
#include "Component.h"
#include "Observer.h"

class AchievementComponent : public dae::Component , public Observer
{
public:
	AchievementComponent(dae::GameObject* pGameObject);
	~AchievementComponent();

	virtual void Notify(const dae::GameObject& gameObject, const Event& action) override;

private:
	// Defining our achievements
	enum EAchievements
	{
		ACH_WIN_ONE_GAME = 0,
		ACH_WIN_100_GAMES = 1,
		ACH_TRAVEL_FAR_ACCUM = 2,
		ACH_TRAVEL_FAR_SINGLE = 3,
	};

	// Achievement array which will hold data about the achievements and their state
	//Achievement_t* m_Achievements = new Achievement_t[]{
	//_ACH_ID(ACH_WIN_ONE_GAME, "Winner"),
	//_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
	//_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
	//_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter"),
	//};
	//int m_AchievmentCount{ 4 };

	// Global access to Achievements object
	//CSteamAchievements* m_pSteamAchievements = NULL;
};

