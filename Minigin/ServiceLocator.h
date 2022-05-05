#pragma once
#include "AudioSystem.h"
#include "NullAudioSystem.h"

class ServiceLocator
{
public:
	ServiceLocator() = delete;
	~ServiceLocator() = delete;

	ServiceLocator(const ServiceLocator& other) = delete;
	ServiceLocator(ServiceLocator&& other) = delete;
	ServiceLocator& operator=(const ServiceLocator& other) = delete;
	ServiceLocator& operator=(ServiceLocator&& other) = delete;

	static void Initialize();
	static void Cleanup();

	static AudioSystem* GetAudio();
	static void ProvideAudio(AudioSystem* pAudioSystem);

private:
	static AudioSystem* m_pAudioService;
	static NullAudioSystem m_NullService;
};

