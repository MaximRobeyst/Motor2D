#pragma once
#include <SDL_mixer.h>

namespace dae
{

	class AudioClip
	{
	public:
		AudioClip(const std::string& path);
		~AudioClip();

		void Load();
		bool Play();
		void SetVolume(int volume);
		int GetVolume();

		bool IsLoaded();

	private:
		std::string m_Path{};
		Mix_Chunk* m_pChunk;
	};
}

