#pragma once
namespace dae
{

	class AudioClip
	{
	public:
		AudioClip(const std::string& path);
		~AudioClip();

		void Load();
		bool Play();
		void SetVolume();
		int GetVolume();

	private:
		std::string m_Path{};
	};
}

