#pragma once
#include "Component.h"

namespace dae
{
	class TransformComponent;
	class ColliderComponent :public Component
	{
	public:
		ColliderComponent(dae::GameObject* pGameObject);
		ColliderComponent(dae::GameObject* pGameObject, float width, float height);

		void Update() override;
		void Render() const override;

	private:
		//TODO: Make a shape struct
		float m_Width{};
		float m_Height{};
		glm::vec2 m_Center{};

		TransformComponent* m_pTransform;
	};
}

