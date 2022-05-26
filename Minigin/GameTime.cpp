#include "MiniginPCH.h"
#include "GameTime.h"

GameTime* GameTime::m_pInstance = nullptr;

GameTime* GameTime::GetInstance()
{
    if (m_pInstance == nullptr) m_pInstance = new GameTime{};

    return m_pInstance;
}

float GameTime::GetElapsed() const
{
    return m_ElapsedTime;
}

void GameTime::SetElapsed(float elapsedTime)
{
    m_ElapsedTime = elapsedTime;
}

int GameTime::GetFPS() const
{
    return static_cast<int>(1.f / m_ElapsedTime);
}
