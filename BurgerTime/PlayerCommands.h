#pragma once
#include <Command.h>

class InteractComponent;
class InteractCommand : public Command
{
public:
	InteractCommand();
	InteractCommand(InteractComponent* pInteractComponent);
	void Execute() override;

private:
	InteractComponent* m_pInteractComponent;
	

};

