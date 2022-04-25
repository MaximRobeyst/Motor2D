#include "MiniginPCH.h"
#include "ServiceLocator.h"

void ServiceLocator::Initialize()
{
	m_pAudioService = &m_NullService;
}

AudioSystem* ServiceLocator::GetAudio()
{
	return m_pAudioService;
}

void ServiceLocator::ProvideAudio(AudioSystem* pAudioSystem)
{

	if (pAudioSystem == nullptr)
		m_pAudioService = &m_NullService;
	else
		m_pAudioService = pAudioSystem;
}
