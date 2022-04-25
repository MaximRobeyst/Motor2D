#include "MiniginPCH.h"
#include "AudioClip.h"

#include "sdl"

dae::AudioClip::AudioClip(const std::string& path)
	:m_Path{path}
{
}

dae::AudioClip::~AudioClip()
{
	m_chunk = Mix_Load();
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
