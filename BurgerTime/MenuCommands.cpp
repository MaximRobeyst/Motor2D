#include "MenuCommands.h"
#include "GameStateManager.h"
#include "Level1State.h"
#include "MenuCommands.h"
#include "MenuComponent.h"

void SwitchStateCommand::Execute()
{
	GameStateManager::GetInstance().SwitchGameState(new Level1State());
}

PressButtonCommand::PressButtonCommand(MenuComponent* pMenuComp)
	: m_pMenuComponent{pMenuComp}
{
}

void PressButtonCommand::Execute()
{
	m_pMenuComponent->PressButton();
}
