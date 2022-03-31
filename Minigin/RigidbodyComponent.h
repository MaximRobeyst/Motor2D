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
		RigidbodyComponent(dae::GameObject* pGameobject);

		void Update() override;
		void Render() const override;

	private:
		b2Body* m_pBody;

		TransformComponent* m_pTransformComponent;
		ColliderComponent* m_pColliderComponent;

		std::shared_ptr<b2World> m_pWorld;
	};
}

