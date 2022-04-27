#pragma once
#include "AudioSystem.h"
#include "NullAudioSystem.h"

class ServiceLocator
{
public:

	static void Initialize();
	static void Cleanup();

	static AudioSystem* GetAudio();
	static void ProvideAudio(AudioSystem* pAudioSystem);

private:
	static AudioSystem* m_pAudioService;
	static NullAudioSystem m_NullService;
};

