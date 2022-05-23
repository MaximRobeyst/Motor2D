#include "MenuCommands.h"
#include "GameStateManager.h"
#include "Level1State.h"
#include "MenuCommands.h"
#include "MenuComponent.h"
#include "MainMenuState.h"

void SwitchMenuStateCommand::Execute()
{
	GameStateManager::GetInstance().SwitchGameState(new MainMenuState());
}

PressButtonCommand::PressButtonCommand(MenuComponent* pMenuComp)
	: m_pMenuComponent{pMenuComp}
{
}

void PressButtonCommand::Execute()
{
	m_pMenuComponent->PressButton();
}

ChangePointerCommand::ChangePointerCommand(MenuComponent* pMenuComp, int change)
	: m_pMenuComponent{pMenuComp}
	, m_Change{change}
{
}

void ChangePointerCommand::Execute()
{
	m_pMenuComponent->SwitchSelection(m_Change);
}
