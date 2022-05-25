#pragma once
#include <Component.h>

#include <vector>

class UIButtonComponent;
class MenuComponent : public dae::Component
{
public:
	MenuComponent() = default;
	MenuComponent(dae::GameObject* pGameobject, dae::GameObject* pPointer);

	void Start() override;

	virtual void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
	virtual void Deserialize(dae::GameObject* pGameobject, rapidjson::Value& value) override;

	void SwitchSelection(int i);
	void PressButton();

private:
	std::vector<UIButtonComponent*> m_pButtonObjects;
	dae::GameObject * m_pMenuPointer;
	int m_CurrentSelection{};
};

