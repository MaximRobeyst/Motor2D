#pragma once
#include "AudioSystem.h"

class NullAudioSystem : public AudioSystem
{
public:
	virtual void PlaySound(int) override {};
	virtual void StopSound(int) override {};
	virtual void StopAllSounds() override	{};

	virtual void SetVolume(int) override {};

	virtual void AddAudioClip(const std::string&) override {};
};

