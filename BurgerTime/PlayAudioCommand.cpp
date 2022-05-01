#include "PlayAudioCommand.h"
#include <ServiceLocator.h>

void PlayAudioCommand::Execute()
{
	//ServiceLocator::GetAudio()->PlaySound(0);

	ServiceLocator::GetAudio()->PlaySound("../Data/Audio/eat_fruit.wav");
	
}

void StopAudioCommand::Execute()
{
	ServiceLocator::GetAudio()->StopAllSounds();
}

VolumeChangeCommand::VolumeChangeCommand(int volumeChange)
	: m_VolumeChange{volumeChange}
{}

void VolumeChangeCommand::Execute()
{
	ServiceLocator::GetAudio()->SetVolume(ServiceLocator::GetAudio()->GetVolume() + m_VolumeChange);
}

void ResumeAudioCommand::Execute()
{
	ServiceLocator::GetAudio()->ResumeAllSounds();
}
