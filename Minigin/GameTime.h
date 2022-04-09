#pragma once
class GameTime
{
public:
	static GameTime* GetInstance();

	virtual ~GameTime() = default;
	GameTime(const GameTime & other) = delete;
	GameTime(GameTime && other) = delete;
	GameTime& operator=(const GameTime & other) = delete;
	GameTime& operator=(GameTime && other) = delete;

	float GetElapsed() const;
	void SetElapsed(float elapsedTime);

	int GetFPS() const;
private:
	GameTime() = default;

	float m_ElapsedTime;
	static GameTime* m_pInstance;
};

