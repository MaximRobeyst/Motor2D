#include "MiniginPCH.h"
#include "Time.h"

float Time::m_ElapsedTime = 0.f;
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
