#pragma once
#include "AudioSystem.h"

class SDLAudioSystem : public AudioSystem
{
public:
	virtual void PlaySound(int id) override;
	virtual void StopSound(int id) override;
	virtual void StopAllSounds() override;

	virtual void SetVolume(int volume) override;

};

