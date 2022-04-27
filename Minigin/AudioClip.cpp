#include "MiniginPCH.h"
#include "AudioClip.h"

dae::AudioClip::AudioClip(const std::string& path)
	:m_Path{path}
{
}

dae::AudioClip::~AudioClip()
{
	if(m_pChunk)
		Mix_FreeChunk(m_pChunk);
}

void dae::AudioClip::Load()
{
	m_pChunk = Mix_LoadWAV(m_Path.c_str());
	if (m_pChunk == nullptr)
	{
		std::cerr << "audioclip " << m_Path << " failed to load, SDL_Mixer error " << Mix_GetError() << std::endl;
	}
}

bool dae::AudioClip::Play()
{
	if (!IsLoaded()) return false;

	int channel{ Mix_PlayChannel(-1, m_pChunk, 0) };
	return channel == -1 ? false : true;
}

void dae::AudioClip::SetVolume(int volume)
{
	if (IsLoaded())
		Mix_VolumeChunk(m_pChunk, volume);
}

int dae::AudioClip::GetVolume()
{
	if (IsLoaded())
		return Mix_VolumeChunk(m_pChunk, -1);

	return 0;
}

bool dae::AudioClip::IsLoaded()
{
	return m_pChunk != nullptr;
}
