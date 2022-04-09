#pragma once
#include "Component.h"
#include <box2d.h>

// https://box2d.org/documentation/index.html
namespace dae
{
	class TransformComponent;
	class ColliderComponent;
	class RigidbodyComponent : public Component
	{
	public:
		struct RigidBody
		{
			b2Vec2 velocity;
		};

		RigidbodyComponent(dae::GameObject* pGameobject, b2BodyType bodyType = b2_dynamicBody, float density = 1.0f, float friction = 0.3f);

		void Update() override;
		void Render() const override;

		void SetVelocity(const glm::vec2& velocity);
		void AddForce(const glm::vec2& force);

		RigidBody& GetRigidbody();

	private:
		b2Body* m_pBody;

		TransformComponent* m_pTransformComponent;
		ColliderComponent* m_pColliderComponent;

		std::shared_ptr<b2World> m_pWorld;

		RigidBody m_RigidBody{};

		float m_Density{};
		float m_Friction{};
	};
}

