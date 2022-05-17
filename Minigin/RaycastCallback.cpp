#include "MiniginPCH.h"
#include "RaycastCallback.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"

float dae::RaycastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{
    m_Hit.pHitObject = reinterpret_cast<dae::RigidbodyComponent*>(fixture->GetBody()->GetUserData().pointer)->GetGameObject();
    m_Hit.point = glm::vec2{ point.x, point.y };
    m_Hit.normal = glm::vec2{ normal.x, normal.y };
    return fraction;
}

dae::RaycastHit dae::RaycastCallback::GetLatestHit() const
{
    return m_Hit;
}
