#include "MiniginPCH.h"
#include "SDLAudioSystem.h"

#include <SDL_mixer.h>

struct AudioDesc
{
	std::string path;
	int loops;
	bool music;
};

class SDLAudioSystem::SDLAudioSystemImpl final
{
public:
	SDLAudioSystemImpl();
	~SDLAudioSystemImpl();


	void PlaySound(const std::string& id, int loops = 0, bool music = true);
	void StopAllSounds() ;
	void ResumeAllSounds();

	void SetVolume(int volume);
	int GetVolume();

private:
	void ProcessQueue();

	//std::queue<dae::AudioClip*> m_pAudioQueue;
	std::queue<AudioDesc> m_pAudioQueue;

	bool m_Running;
	std::thread m_AudioThread;
	std::mutex m_AudioMutex;
	std::condition_variable m_ConditionVariable;

	int m_CurrentVolume{ SDL_MIX_MAXVOLUME };
};

SDLAudioSystem::SDLAudioSystemImpl::SDLAudioSystemImpl()
	: m_Running{ true }
	, m_AudioThread{ &SDLAudioSystemImpl::ProcessQueue, this }
{
	int flags = MIX_INIT_OGG | MIX_INIT_MOD;
	int init = Mix_Init(flags);
	if ((init & flags) != flags)
	{
		std::cerr << "Mix init: failed to init required ogg and mod support!" << std::endl;
		std::cerr << "Mx_Init: " << Mix_GetError() << std::endl;
	}

	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 4, 1024);
}

SDLAudioSystem::SDLAudioSystemImpl::~SDLAudioSystemImpl()
{
	StopAllSounds();

	Mix_CloseAudio();
	Mix_Quit();

	m_ConditionVariable.notify_one();
	m_Running = false;
	m_AudioThread.join();
}

void SDLAudioSystem::SDLAudioSystemImpl::PlaySound(const std::string& id, int loops, bool music)
{
	std::unique_lock<std::mutex> lock{ m_AudioMutex };
	m_pAudioQueue.emplace(AudioDesc{id, loops, music});
	m_ConditionVariable.notify_one();
}

void SDLAudioSystem::SDLAudioSystemImpl::StopAllSounds()
{
	while (!m_pAudioQueue.empty())
		m_pAudioQueue.pop();

	Mix_Pause(-1);
	Mix_PauseMusic();
}

void SDLAudioSystem::SDLAudioSystemImpl::ResumeAllSounds()
{
	Mix_Resume(-1);
	Mix_ResumeMusic();
}

void SDLAudioSystem::SDLAudioSystemImpl::SetVolume(int volume)
{
	m_CurrentVolume = volume;
	Mix_VolumeMusic(volume);
}

int SDLAudioSystem::SDLAudioSystemImpl::GetVolume()
{
	return m_CurrentVolume;
}

void SDLAudioSystem::SDLAudioSystemImpl::ProcessQueue()
{
	while (m_Running)
	{
		{
			std::unique_lock<std::mutex> lock{ m_AudioMutex };
			while (!m_pAudioQueue.empty())
			{
				auto audioDesc = m_pAudioQueue.front();
				m_pAudioQueue.pop();

				if (audioDesc.music)
				{
					auto music = Mix_LoadMUS(audioDesc.path.c_str());
					Mix_VolumeMusic(m_CurrentVolume);
					Mix_PlayMusic(music, audioDesc.loops);
				}
				else
				{
					auto chunk = Mix_LoadWAV(audioDesc.path.c_str());
					Mix_VolumeChunk(chunk, m_CurrentVolume);
					Mix_PlayChannel(-1, chunk, audioDesc.loops);
				}
			}

			if (m_pAudioQueue.empty())
				m_ConditionVariable.wait(lock);
		}
	}
}


SDLAudioSystem::SDLAudioSystem()
	: m_pImpl{ std::make_unique<SDLAudioSystemImpl>() }
{
}

SDLAudioSystem::~SDLAudioSystem()
{
}

void SDLAudioSystem::PlaySound(const std::string& id, int loops, bool music)
{
	//m_pAudioQueue.emplace(new dae::AudioClip(id));
	m_pImpl->PlaySound(id, loops, music);
}

void SDLAudioSystem::StopAllSounds()
{
	m_pImpl->StopAllSounds();
}

void SDLAudioSystem::ResumeAllSounds()
{
	m_pImpl->ResumeAllSounds();
}

void SDLAudioSystem::SetVolume(int volume)
{
	m_pImpl->SetVolume(volume);
}

int SDLAudioSystem::GetVolume()
{
	return m_pImpl->GetVolume();
}