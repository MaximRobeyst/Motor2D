#pragma once
#include "box2d.h"

namespace dae 
{
	class GameObject;

	struct RaycastHit
	{
		GameObject* pHitObject{nullptr};
		glm::vec2 point;
		glm::vec2 normal;
		float fraction;
		bool hit{false};
	};

	class RaycastCallback : public b2RayCastCallback
	{
	public:
		RaycastCallback(uint16 layermask = 0x0000);

		virtual float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
			const b2Vec2& normal, float fraction);

		RaycastHit GetLatestHit() const;
	private:
		RaycastHit m_Hit;
		uint16 m_Layermask{};

	};
}


