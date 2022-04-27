#include "MiniginPCH.h"
#include "AudioClip.h"

dae::AudioClip::AudioClip(const std::string& path)
	:m_Path{path}
{
}

dae::AudioClip::~AudioClip()
{
}

void dae::AudioClip::Load()
{
}

bool dae::AudioClip::Play()
{
	return false;
}

void dae::AudioClip::SetVolume()
{
}

int dae::AudioClip::GetVolume()
{
	return 0;
}
