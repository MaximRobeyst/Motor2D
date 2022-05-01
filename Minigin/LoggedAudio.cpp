#include "MiniginPCH.h"
#include "LoggedAudio.h"

LoggedAudio::LoggedAudio(AudioSystem* pWrappedAudio)
	: m_pWrappedAudio{pWrappedAudio}
{
}

LoggedAudio::~LoggedAudio()
{
	delete m_pWrappedAudio;
}

void LoggedAudio::PlaySound(const std::string& id, int loops)
{
	m_pWrappedAudio->PlaySound(id, loops);
	Log("Play sound" + (id));
}

void LoggedAudio::StopAllSounds()
{
	m_pWrappedAudio->StopAllSounds();
	Log("All sounds have been stopped");
}

int LoggedAudio::GetVolume()
{
	int currentVolume =  m_pWrappedAudio->GetVolume();
	Log("Current volume is: " + std::to_string(currentVolume));
	return currentVolume;
}

void LoggedAudio::SetVolume(int volume)
{
	m_pWrappedAudio->SetVolume(volume);
	Log("Audio volume has been set to " + std::to_string(volume));
}

void LoggedAudio::Log(const std::string& message)
{
	std::cout << message << std::endl;
}
