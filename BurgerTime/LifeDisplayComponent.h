#pragma once
#include "Component.h"
#include "Observer.h"
#include "Font.h"
#include "TextObject.h"

enum class Event;
class LifeDisplayComponent : public dae::Component, public Observer
{
public:
	LifeDisplayComponent(dae::GameObject* pGameObject, int number, const std::string& extraDisplayText = "");

	void Notify(const dae::GameObject& gameObject, const Event& action);
	void ChangeText(int number);
private:
	dae::TextComponent* m_pTextComponent;
	std::string m_ExtraDisplayText;
};

