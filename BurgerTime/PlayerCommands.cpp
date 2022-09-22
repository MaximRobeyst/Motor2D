#include "PlayerCommands.h"
#include "InteractComponent.h"

InteractCommand::InteractCommand(InteractComponent* pInteractComponent)
	: m_pInteractComponent{ pInteractComponent }
{
}

void InteractCommand::Execute()
{
	m_pInteractComponent->InteractWithCurrentInteractable();
}
