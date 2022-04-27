#include "MiniginPCH.h"
#include "SDLAudioSystem.h"

SDLAudioSystem::SDLAudioSystem()
{
	int flags = MIX_INIT_OGG | MIX_INIT_MOD;
	int init = Mix_Init(flags);
	if (  (init&flags) != flags)
	{
		std::cerr << "Mix init: failed to init required ogg and mod support!" << std::endl;
		std::cerr << "Mx_Init: " << Mix_GetError() << std::endl;
	}

	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
}

SDLAudioSystem::~SDLAudioSystem()
{
	for (dae::AudioClip* clip : m_pAudioClips)
		delete clip;

	m_pAudioClips.clear();

	Mix_CloseAudio();
	Mix_Quit();
}

void SDLAudioSystem::PlaySound(int id)
{
	if (!m_pAudioClips[id]->IsLoaded())
		m_pAudioClips[id]->Load();

	m_pAudioClips[id]->Play();
}

void SDLAudioSystem::StopSound(int /*id*/)
{
}

void SDLAudioSystem::StopAllSounds()
{
}

void SDLAudioSystem::SetVolume(int /*volume*/)
{
}

void SDLAudioSystem::AddAudioClip(const std::string& path)
{
	m_pAudioClips.emplace_back(new dae::AudioClip(path));
}
