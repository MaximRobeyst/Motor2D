#pragma once
#include "box2d.h"

namespace dae 
{
	class GameObject;

	struct RaycastHit
	{
		GameObject* pHitObject;
		glm::vec2 point;
		glm::vec2 normal;
		float fraction;
	};

	class RaycastCallback : public b2RayCastCallback
	{
	public:
		RaycastCallback(b2World* pWorld);

		void Raycast(const glm::vec2& startpos, const glm::vec2& direction, float distance, RaycastHit& hit);

		virtual float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
			const b2Vec2& normal, float fraction);

		RaycastHit GetLatestHit() const;
	private:
		b2World* m_pWorld;

		RaycastHit m_Hit;

	};
}


