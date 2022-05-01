#pragma once
#include <string>

class AudioSystem
{

public:
	virtual ~AudioSystem() = default;

	virtual void PlaySound(const std::string& soundId, int loops = 0) = 0;
	virtual void StopAllSounds() = 0;

	virtual void SetVolume(int volume) = 0;
	virtual int GetVolume() = 0;
};

