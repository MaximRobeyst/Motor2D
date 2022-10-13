#include "PlayerCommands.h"
#include "InteractComponent.h"

dae::Creator<Command, InteractCommand> g_InteractCommand;

InteractCommand::InteractCommand()
{
}

InteractCommand::InteractCommand(InteractComponent* pInteractComponent)
	: m_pInteractComponent{ pInteractComponent }
{
}

void InteractCommand::Execute()
{
	if(m_pInteractComponent != nullptr)
		m_pInteractComponent->InteractWithCurrentInteractable();
}
