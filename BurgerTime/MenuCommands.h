#pragma once
#include <Command.h>

class SwitchStateCommand : public Command
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
