#pragma once
class Time
{
public:
	static Time* GetInstance();

	virtual ~Time() = default;
	Time(const Time & other) = delete;
	Time(Time && other) = delete;
	Time& operator=(const Time & other) = delete;
	Time& operator=(Time && other) = delete;

	float GetElapsed() const;
	void SetElapsed(float elapsedTime);

	int GetFPS() const;
private:
	Time() = default;

	float m_ElapsedTime;
	static Time* m_pInstance;
};

