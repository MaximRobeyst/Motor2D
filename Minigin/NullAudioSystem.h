#pragma once
#include "AudioSystem.h"

class NullAudioSystem : public AudioSystem
{
public:
	virtual void PlaySound(const std::string& /*id*/, int, bool) override {};
	virtual void StopAllSounds() override	{};
	virtual void ResumeAllSounds() override {};

	virtual int GetVolume() override { return 0; };
	virtual void SetVolume(int) override {};
};

