#include "MiniginPCH.h"
#include "Command.h"

#include "LifeComponent.h"

void FireCommand::Execute()
{
	std::cout << "Fire" << std::endl;
}

void DuckCommand::Execute()
{
	std::cout << "Dock" << std::endl;
}

void JumpCommand::Execute()
{
	std::cout << "Jump" << std::endl;
}

void FartCommand::Execute()
{
	std::cout << "Fart" << std::endl;
}

KillCommand::KillCommand(LifeComponent* pLifeTarget)
	:m_pLifeTarget{ pLifeTarget }
{
}

void KillCommand::Execute()
{
	m_pLifeTarget->Hit();
}
