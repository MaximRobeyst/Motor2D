#pragma once
#include "Component.h"
#include "Observer.h"
#include "Font.h"
#include "TextObject.h"

enum class Event;

class LifeComponent;
class LifeDisplayComponent : public dae::Component, public Observer
{
public:
	LifeDisplayComponent() = default;
	LifeDisplayComponent(dae::GameObject* pGameObject, int number, const std::string& extraDisplayText = "");

	void Start() override;
	void Render() const override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;


	void Notify(const dae::GameObject& gameObject, const Event& action);

	void ChangeText(int number);
private:
	dae::TextComponent* m_pTextComponent;
	LifeComponent* m_pLifeComponent;
	std::string m_ExtraDisplayText;
};

