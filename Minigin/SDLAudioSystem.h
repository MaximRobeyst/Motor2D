#pragma once
#include "AudioSystem.h"

#include <thread>
#include <queue>
#include <mutex>

class SDLAudioSystem final : public AudioSystem
{
	class SDLAudioSystemImpl;
	std::unique_ptr<SDLAudioSystemImpl> m_pImpl;

public:
	SDLAudioSystem();
	~SDLAudioSystem();

	virtual void PlaySound(const std::string& id) override;
	virtual void StopAllSounds() override;

	virtual void SetVolume(int volume) override;
	virtual int GetVolume() override;

private:

};

