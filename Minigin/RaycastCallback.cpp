#include "MiniginPCH.h"
#include "RaycastCallback.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"

dae::RaycastCallback::RaycastCallback(b2World* pWorld)
    : m_pWorld{pWorld}
{
}

void dae::RaycastCallback::Raycast(const glm::vec2& startpos, const glm::vec2& direction, float distance, RaycastHit& hit)
{
    b2Vec2 rayStart{ startpos.x , startpos.y };
    b2Vec2 rayEnd{ startpos.x + (direction.x * distance) , startpos.y + (direction.y * distance)};

    m_pWorld->RayCast(this, rayStart, rayEnd);
    hit = m_Hit;
}

float dae::RaycastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{
    m_Hit.pHitObject = reinterpret_cast<dae::RigidbodyComponent*>(fixture->GetBody()->GetUserData().pointer)->GetGameObject();
    m_Hit.point = glm::vec2{ point.x, point.y };
    m_Hit.normal = glm::vec2{ normal.x, normal.y };
    m_Hit.fraction = fraction;
    return fraction;
}

dae::RaycastHit dae::RaycastCallback::GetLatestHit() const
{
    return m_Hit;
}
