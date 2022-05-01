#pragma once
#include "AudioSystem.h"

class LoggedAudio final :public AudioSystem
{
public:
	LoggedAudio(AudioSystem* pWrappedAudio);
	~LoggedAudio();

	LoggedAudio(const LoggedAudio&) = delete;
	LoggedAudio(LoggedAudio&&) = delete;
	LoggedAudio& operator=(const LoggedAudio&) = delete;
	LoggedAudio& operator=(LoggedAudio&&) = delete;

	virtual void PlaySound(const std::string& id) override;
	virtual void StopAllSounds() override;

	virtual int GetVolume() override;
	virtual void SetVolume(int) override;

private:
	void Log(const std::string& message);


	AudioSystem* m_pWrappedAudio;

};

