#pragma once
#include <Command.h>

namespace dae
{
	class Scene;
}

class SwitchMenuStateCommand : public Command
{
public:
	void Execute() override;
};

class MenuComponent;
class PressButtonCommand : public Command
{
public:
	PressButtonCommand() = default;
	PressButtonCommand(MenuComponent* pMenuComp);
	void Execute() override;

	virtual void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>&) override;
	virtual void Deserialize(rapidjson::Value&, dae::Scene* pScene) override;
private:
	MenuComponent* m_pMenuComponent;
};

class ChangePointerCommand : public Command
{
public:
	ChangePointerCommand() = default;
	ChangePointerCommand(MenuComponent* pMenuComp, int change = 1);
	void Execute() override;

	virtual void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>&) override;
	virtual void Deserialize(rapidjson::Value&, dae::Scene* pScene) override;
private:
	MenuComponent* m_pMenuComponent{};
	int m_Change{};
};