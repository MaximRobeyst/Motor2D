#include "PlayAudioCommand.h"
#include <ServiceLocator.h>

dae::Creator<Command, PlayAudioCommand> g_PlayerAudioComandCreator;

void PlayAudioCommand::Execute()
{
	//ServiceLocator::GetAudio()->PlaySound(0);

	ServiceLocator::GetAudio()->PlaySound("../Data/Audio/eat_fruit.wav");
	
}

dae::Creator<Command, StopAudioCommand> g_StopAudioCommandCreator;

void StopAudioCommand::Execute()
{
	ServiceLocator::GetAudio()->StopAllSounds();
}

dae::Creator<Command, VolumeChangeCommand> g_VolumeChangeCommandCreator;

VolumeChangeCommand::VolumeChangeCommand(int volumeChange)
	: m_VolumeChange{volumeChange}
{}

void VolumeChangeCommand::Execute()
{
	ServiceLocator::GetAudio()->SetVolume(ServiceLocator::GetAudio()->GetVolume() + m_VolumeChange);
}

dae::Creator<Command, ResumeAudioCommand> g_ResumeAudioCommandCreator;

void ResumeAudioCommand::Execute()
{
	ServiceLocator::GetAudio()->ResumeAllSounds();
}
