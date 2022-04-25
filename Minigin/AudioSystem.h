#pragma once
class AudioSystem
{

public:
	virtual ~AudioSystem() = default;

	virtual void PlaySound(int soundId) = 0;
	virtual void StopSound(int soundId) = 0;
	virtual void StopAllSounds() = 0;

	virtual void SetVolume(int volume) = 0;
};

