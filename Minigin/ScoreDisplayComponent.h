#pragma once
#include "Component.h"
#include "Observer.h"
#include "TextObject.h"

class Subject;
class ScoreDisplayComponent : public dae::Component, public Observer
{
public:
	ScoreDisplayComponent(dae::GameObject* pGameObject, int number, const std::string& extraDisplayText = "");

	void Notify(const dae::GameObject& gameObject, const Event& action);
	void ChangeText(int number);

	std::unique_ptr<Subject>& GetSubject();
private:
	dae::TextComponent* m_pTextComponent;
	std::string m_ExtraDisplayText;

	int m_Score{};

	std::unique_ptr<Subject> m_pSubject;
};
