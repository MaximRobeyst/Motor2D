#include "MiniginPCH.h"
#include "SDLAudioSystem.h"

#include <SDL_mixer.h>

class AudioClip final
{
public:
	AudioClip(const std::string& path);
	~AudioClip();

	void Load();
	int Play();
	void SetVolume(int volume);
	int GetVolume();

	bool IsLoaded();

private:
	std::string m_Path{};
	Mix_Chunk* m_pChunk;
};

AudioClip::AudioClip(const std::string& path)
	:m_Path{ path }
{
}

AudioClip::~AudioClip()
{
	if (m_pChunk)
		Mix_FreeChunk(m_pChunk);
}

void AudioClip::Load()
{
	m_pChunk = Mix_LoadWAV(m_Path.c_str());
	if (m_pChunk == nullptr)
	{
		std::cerr << "audioclip " << m_Path << " failed to load, SDL_Mixer error " << Mix_GetError() << std::endl;
	}
}

int AudioClip::Play()
{
	if (!IsLoaded()) return false;

	int channel{ Mix_PlayChannel(-1, m_pChunk, 0) };
	return channel;
}

void AudioClip::SetVolume(int volume)
{
	if (IsLoaded())
		Mix_VolumeChunk(m_pChunk, volume);
}

int AudioClip::GetVolume()
{
	if (IsLoaded())
		return Mix_VolumeChunk(m_pChunk, -1);

	return 0;
}

bool AudioClip::IsLoaded()
{
	return m_pChunk != nullptr;
}

class SDLAudioSystem::SDLAudioSystemImpl final
{
public:
	SDLAudioSystemImpl();
	~SDLAudioSystemImpl();


	void PlaySound(const std::string& id);
	void StopAllSounds() ;
	void SetVolume(int volume);
	int GetVolume();

private:
	void ProcessQueue();

	//std::queue<dae::AudioClip*> m_pAudioQueue;
	std::queue<std::string> m_pAudioQueue;

	bool m_Running;
	std::thread m_AudioThread;
	std::mutex m_AudioMutex;
	std::condition_variable m_ConditionVariable;
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

	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
}

SDLAudioSystem::SDLAudioSystemImpl::~SDLAudioSystemImpl()
{
	Mix_CloseAudio();
	Mix_Quit();

	m_ConditionVariable.notify_one();
	m_Running = false;
	m_AudioThread.join();
}

void SDLAudioSystem::SDLAudioSystemImpl::PlaySound(const std::string& id)
{
	std::unique_lock<std::mutex> lock{ m_AudioMutex };
	m_pAudioQueue.emplace(id);
	m_ConditionVariable.notify_one();
}

void SDLAudioSystem::SDLAudioSystemImpl::StopAllSounds()
{
}

void SDLAudioSystem::SDLAudioSystemImpl::SetVolume(int /*volume*/)
{
}

int SDLAudioSystem::SDLAudioSystemImpl::GetVolume()
{
	return 0;
}

void SDLAudioSystem::SDLAudioSystemImpl::ProcessQueue()
{
	std::queue<std::pair<int, AudioClip*>> playedClips;
	while (m_Running)
	{
		{
			std::unique_lock<std::mutex> lock{ m_AudioMutex };
			while (!m_pAudioQueue.empty())
			{

				auto clip = new AudioClip(m_pAudioQueue.front());
				m_pAudioQueue.pop();

				clip->Load();
				int channel = clip->Play();

				playedClips.push(std::make_pair(channel, clip));
			}

			if (m_pAudioQueue.empty() && playedClips.empty())
				m_ConditionVariable.wait(lock);
		}

		// Remove audio clips if they arent playing
		while (!playedClips.empty())
		{
			auto pair = playedClips.front();
			if (Mix_Playing(pair.first) == 0)
			{
				delete pair.second;
				playedClips.pop();
			}
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

void SDLAudioSystem::PlaySound(const std::string& id)
{
	//m_pAudioQueue.emplace(new dae::AudioClip(id));
	m_pImpl->PlaySound(id);
}

void SDLAudioSystem::StopAllSounds()
{
	m_pImpl->StopAllSounds();
}

void SDLAudioSystem::SetVolume(int volume)
{
	m_pImpl->SetVolume(volume);
}

int SDLAudioSystem::GetVolume()
{
	return m_pImpl->GetVolume();
}