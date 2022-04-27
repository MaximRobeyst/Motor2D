#include "MiniginPCH.h"
#include "ServiceLocator.h"

AudioSystem* ServiceLocator::m_pAudioService{ nullptr };
NullAudioSystem ServiceLocator::m_NullService{};

void ServiceLocator::Initialize()
{
	m_pAudioService = &m_NullService;
}

void ServiceLocator::Cleanup()
{
	if (m_pAudioService != nullptr)
		delete m_pAudioService;
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
