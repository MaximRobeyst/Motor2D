#include "MiniginPCH.h"
#include "Command.h"

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
