#pragma once
#include <Component.h>
#include <string>

namespace dae 
{
	class TextComponent;
}

class LeaderboardComponent : public dae::Component
{
public:
	LeaderboardComponent() = default;
	LeaderboardComponent(dae::GameObject* pGameobject, int targetScore);

	void Start() override;
	void Update() override;

	void AddLetter(char letter);
	void RemoveLetter();

private:
	int m_PlayerScore{};
	bool m_Ready{ false };

	std::string m_CurrentName;
	dae::TextComponent* m_pTargetText;
};

