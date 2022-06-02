#include "MiniginPCH.h"
#include "RaycastCallback.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"

dae::RaycastCallback::RaycastCallback(uint16 layermask)
    : m_Layermask{layermask}
{
}

float dae::RaycastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{
    if (m_Layermask != 0 && (fixture->GetFilterData().categoryBits & m_Layermask) != 0)
        return -1;

    m_Hit.pHitObject = reinterpret_cast<dae::RigidbodyComponent*>(fixture->GetBody()->GetUserData().pointer)->GetGameObject();
    m_Hit.point = glm::vec2{ point.x, point.y };
    m_Hit.normal = glm::vec2{ normal.x, normal.y };
    m_Hit.hit = true;

    return fraction;
}

dae::RaycastHit dae::RaycastCallback::GetLatestHit() const
{
    return m_Hit;
}
