#pragma once
#include <Command.h>

class PlayAudioCommand : public Command
{
public:
	void Execute() override;
};

class StopAudioCommand : public Command
{
public:
	void Execute() override;
};

class ResumeAudioCommand : public Command
{
public:
	void Execute() override;
};

class VolumeChangeCommand : public Command
{
public:
	VolumeChangeCommand(int volumeChange);
	void Execute() override;

private:
	int m_VolumeChange;
};
