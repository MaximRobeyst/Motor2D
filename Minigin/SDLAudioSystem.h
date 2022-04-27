#pragma once
#include "AudioSystem.h"

#include "AudioClip.h"

class SDLAudioSystem final : public AudioSystem
{
public:
	SDLAudioSystem();
	~SDLAudioSystem();

	virtual void PlaySound(int id) override;
	virtual void StopSound(int id) override;
	virtual void StopAllSounds() override;

	virtual void SetVolume(int volume) override;

	virtual void AddAudioClip(const std::string& path) override;

private:
	std::vector<dae::AudioClip*> m_pAudioClips;

};

