#pragma once
#include <Command.h>

class SwitchMenuStateCommand : public Command
{
public:
	void Execute() override;
};

class MenuComponent;
class PressButtonCommand : public Command
{
public:
	PressButtonCommand(MenuComponent* pMenuComp);
	void Execute() override;
private:
	MenuComponent* m_pMenuComponent;
};

class ChangePointerCommand : public Command
{
public:
	ChangePointerCommand(MenuComponent* pMenuComp, int change = 1);
	void Execute() override;
private:
	MenuComponent* m_pMenuComponent{};
	int m_Change{};
};