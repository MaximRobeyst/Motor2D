#include "SwitchStateCommand.h"
#include "GameStateManager.h"
#include "Level1State.h"

void SwitchStateCommand::Execute()
{
	GameStateManager::GetInstance().SwitchGameState(new Level1State());
}
