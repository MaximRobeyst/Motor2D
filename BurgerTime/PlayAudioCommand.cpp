#include "PlayAudioCommand.h"
#include <ServiceLocator.h>

void PlayAudioCommand::Execute()
{
	ServiceLocator::GetAudio()->PlaySound(0);
}
