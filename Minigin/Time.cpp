#include "MiniginPCH.h"
#include "Time.h"

Time* Time::m_pInstance = nullptr;

Time* Time::GetInstance()
{
    if (m_pInstance == nullptr) m_pInstance = new Time{};

    return m_pInstance;
}

float Time::GetElapsed() const
{
    return m_ElapsedTime;
}

void Time::SetElapsed(float elapsedTime)
{
    m_ElapsedTime = elapsedTime;
}

int Time::GetFPS() const
{
    return static_cast<int>(1.f / (1.f / (1.f / m_ElapsedTime)));
    //return m_Frames / static_cast<int>(m_Time);
}
